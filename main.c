/*
 * File:   main.c
 * Author: Tomas Pikous
 *
 * Created on 2. března 2016, 18:57
 * 
 * LoRa Light Control Module software
 * 
 * LoRa Module pinout * 
 * 1 VDD (closer to PCB edge)
 * 2 MCLR
 * 3 CLK
 * 4 DATA
 * 5 GND
 * 
 */

#define _XTAL_FREQ  64000000

#include <xc.h>
#include "mcc_generated_files/eusart1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/epwm1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char uuid[40];
uint8_t val=0;
uint8_t inc=0;

/**
 * Waits for acknowledge from RN2483 module
 */
void wait_for_ok(void){
    while(uartState!=1){
        __delay_ms(1);
    }
    uartState=0;
}

/**
 * Delay for specified amount of time 
 * Delay time = j*5ms
 * @param j count of 5ms delays
 */
void wait_5ms(int j){
    for(uint8_t i=0;i<j;i++) { __delay_ms(5); }   
}

/**
 * Delay for specified amount of time 
 * Delay time = j*1ms
 * @param j count of 1ms delays
 */
void wait_1ms(int j){
    for(uint8_t i=0;i<j;i++) { __delay_ms(1); }   
}

/**
 * Requests UUID from RN2483 module and save it to variable
 */
void getUUID(){
    printf("sys get hweui\r\n");
    read_line(); 
    strcpy(uuid,line); 
}

/**
 * Process packet
 * Packet must have given length and UUID.
 * @return 
 */
int process_message(){
    //packet length doesn't match
    if(strlen(line)!=32)return 0;
    
    int res = 0;    
    //check if contains specific UUID
    if(strstr(line,uuid)!=0)res++;
    //check if contains broadcast UUID
    if(strstr(line,"FFFFFFFFFFFFFFFF")!=0)res++;
   
    if(res>0){
        //parse illum value 28 29
        char hexval[4];
        sprintf(hexval,"%c%c",line[28],line[29]);
        val = (int)strtol(hexval, NULL, 16);        
        return 1;
    }
    return 0;
}

/**
 * Setup RN2483
 * Set of RN2483 commands (look at datasheet) to set up this module.
 * This setting seems to reasonable mix of quality and message rate 
 */
void InitRadio(void){
    wait_5ms(5); 
    //resets module
    printf("sys reset\r\n");
    wait_5ms(5); 
    //read module version
    printf("sys get ver\r\n");
    read_line(); 
    //read module spread factor
    printf("radio get sf\r\n");
    read_line();  
    //read module bandwidth
    printf("radio get bw\r\n");
    read_line();  
    //read module coding rate
    printf("radio get cr\r\n");
    read_line();  
    //read module preamble length
    printf("radio get prlen\r\n");
    read_line(); 
    //read module transmit power 
    printf("radio get pwr\r\n");
    read_line(); 
    /*
     * pauses mac layer
     * This is needed because of lack of LoRa gateway. 
     * We disable MAC layer to allow modules talk to each other. 
     */
    printf("mac pause\r\n");
    read_line(); 
           
    //set power to -2
    printf("radio set pwr -2\r\n");
    read_line(); 
    //set bandwidth to 500KHz
    printf("radio set bw 500\r\n");
    read_line(); 
    //set spread factor to sf7
    printf("radio set sf sf7\r\n");
    read_line(); 
    //set coding rate to 4/8
    printf("radio set cr 4/8\r\n");
    read_line(); 
    //enable crc
    printf("radio set crc on\r\n");
    read_line();
    //read and save uuid
    getUUID();    
}

/**
 * Main code where RN2483 is initialized and then the module is listening for commands.
 * While it (in partially random intervals) sends heartbeat packets. 
 */
void main(void) {
    //System initialization
    SYSTEM_Initialize(); 	
    INTERRUPT_GlobalInterruptEnable();  
	INTERRUPT_PeripheralInterruptEnable();
    InitRadio(); 
    
    //random delay generation
    srand((int)strtol(uuid, NULL, 0));
    int delay = 2000+(rand()%1000);
    printf("radio set wdt %d\r\n",delay);
    read_line(); 
    
    //start listening
    printf("radio rx 0\r\n");
    read_line(); 
    
    int q=0;
    while(1){
        __delay_ms(1);
        q++;
        
        //packet format
        //1B       8B               1B        1B  1B
        //PREAMBLE UUID             INCREMENT PWM REQ_ID
        //42       0000000000000000 00        00  00
        
        //perform restart when RN2483 is not responding
        if(q>(2*delay)){break;}
        
        //when something arrived from UART
        if(uartState>2){
            read_line();   
            
            //Send heartbeat when packet from another light arrived and 
            //heartbeat delay was exceeded
            if(uartState==3){
                if(q>delay){
                    uartState=5;
                }
            }  
            
            //Message from hub arrived
            if(uartState==4){
                /**
                 * If it's a valid message then set pwm and send ack
                 * when it isn't and heartbeat delay was exceeded send HB packet
                 */
                if(process_message()){      
                    delay = 2000+(rand()%1000);
                    //send ack
                    printf("radio tx 42%s%02X%02X00000001\r\n",uuid,inc++,val); 
                    read_line(); 
                    read_line();
                    //update watchdog
                    printf("radio set wdt %d\r\n",delay);
                    read_line(); 
                    //set pwm
                    EPWM1_LoadDutyValue((uint16_t) val*4);      
                    q=0;
                } else {
                    if(q>delay){
                        uartState=5;
                    }
                }                
            }
            
            //Watchdog expired
            if(uartState==5){
                delay = 2000+(rand()%1000);
                //send HB packet
                printf("radio tx 42%s%02X%02X00000000\r\n",uuid,inc++,val); 
                read_line(); 
                read_line(); 
                //update watchdog
                printf("radio set wdt %d\r\n",delay);
                read_line(); 
                q=0;
            }
              
            //listen for packets
            printf("radio rx 0\r\n");
            read_line(); 
            uartState=0;        
        }   
    }
    return;
}
