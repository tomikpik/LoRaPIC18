/*
 * File:   main.c
 * Author: Tom
 *
 * Created on 2. března 2016, 18:57
 * 
 * 
 * 
 * 1 VDD
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


void wait_for_ok(void){
    while(uartState!=1){
        __delay_ms(1);
    }
    uartState=0;
}

void wait_5ms(int j){
    for(uint8_t i=0;i<j;i++) { __delay_ms(5); }   
}

void wait_1ms(int j){
    for(uint8_t i=0;i<j;i++) { __delay_ms(1); }   
}

void getUUID(){
    printf("sys get hweui\r\n");
    read_line(); 
    strcpy(uuid,line); 
}

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



void main(void) {
    SYSTEM_Initialize(); 	
    INTERRUPT_GlobalInterruptEnable();  
	INTERRUPT_PeripheralInterruptEnable();
    
    
    wait_5ms(5); 
    printf("sys reset\r\n");
    wait_5ms(5); 
    printf("sys get ver\r\n");
    read_line(); 
    printf("radio get sf\r\n");
    read_line();  
    printf("radio get bw\r\n");
    read_line();  
    printf("radio get cr\r\n");
    read_line();  
    printf("radio get prlen\r\n");
    read_line(); 
    printf("radio get pwr\r\n");
    read_line(); 
    printf("mac pause\r\n");
    read_line(); 
           
    printf("radio set pwr -2\r\n");
    read_line(); 
    printf("radio set bw 500\r\n");
    read_line(); 
    printf("radio set sf sf7\r\n");
    read_line(); 
    printf("radio set cr 4/8\r\n");
    read_line(); 
    printf("radio set crc on\r\n");
    read_line();
    
    getUUID();
     
    srand((int)strtol(uuid, NULL, 0));
    int delay = 2000+(rand()%1000);
    printf("radio set wdt %d\r\n",delay);
    read_line(); 
    
   
    /*
    printf("radio set pwr -3\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set mod fsk\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set bt 1.0\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set rxbw 2.6\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    */
            
    printf("radio rx 0\r\n");
    read_line(); 
    
    
    int q=0;
    
    while(1){
        __delay_ms(1);
        q++;
            
            //1B       8B               1B        1B  1B
            //PREAMBLE UUID             INCREMENT PWM REQ_ID
            //42       0000000000000000 00        00  00
        
        if(q>(2*delay)){
            break;
        }
        
        
        
        
        if(uartState>2){
            read_line();   
            
            
            if(uartState==3){
                if(q>delay){
                    uartState=5;
                }
            }  
            
            
            
            if(uartState==4){               
                
                if(process_message()){      
                    delay = 2000+(rand()%1000);
                    wait_1ms((rand()%40)+25);
                    printf("radio tx 42%s%02X%02X00000001\r\n",uuid,inc++,val); 
                    read_line(); 
                    read_line();                 
                    printf("radio set wdt %d\r\n",delay);
                    read_line(); 

                    EPWM1_LoadDutyValue((uint16_t) val*4);      
                    q=0;
                } else {
                    if(q>delay){
                        uartState=5;
                    }
                }                
            }
            
            if(uartState==5){
                delay = 2000+(rand()%1000);
                printf("radio tx 42%s%02X%02X00000000\r\n",uuid,inc++,val); 
                read_line(); 
                read_line(); 
                printf("radio set wdt %d\r\n",delay);
                read_line(); 
                q=0;
            }
              
            printf("radio rx 0\r\n");
            read_line(); 
            uartState=0;        
            
        }
        
                
        
    }
    
    
    return;
}
