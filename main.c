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


void main(void) {
    SYSTEM_Initialize();
 	
    INTERRUPT_GlobalInterruptEnable();
								
	INTERRUPT_PeripheralInterruptEnable();
                                
    __delay_ms(10);
    
    printf("sys reset\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }   
    printf("sys get ver\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio get sf\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio get bw\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio get cr\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio get prlen\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio get pwr\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    
    printf("mac pause\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
           
    printf("radio set pwr -3\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set bw 500\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set sf sf7\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set cr 4/5\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    printf("radio set crc on\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }

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
            
    uint16_t val=0;
    uint16_t sleep=0;
    int a =0;
    while(1){
        /*
        //putch()
        for(uint8_t i=0;i<50;i++) { __delay_ms(10); }
        
        //EPWM1_LoadDutyValue((uint16_t) val);
        
        printf("radio tx 0100%i\r\n",val);
        
        val=val+333;
        if(val>1024)val=0;
        */
        
        __delay_ms(10);
        __delay_ms(10);
        __delay_ms(10);
        __delay_ms(10);
        __delay_ms(10);
        printf("radio tx 1000000000000000000000%04X%d\r\n",val++,uartState);    
        
        
        
    }
    
    
    return;
}
