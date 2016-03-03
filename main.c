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


void main(void) {
    SYSTEM_Initialize();
 								// Enable the Global Interrupts
								INTERRUPT_GlobalInterruptEnable();
								
								// Enable the Peripheral Interrupts
								INTERRUPT_PeripheralInterruptEnable();
                                
    __delay_ms(10);
    
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
             
    printf("radio set pwr 15\r\n");
    for(uint8_t i=0;i<10;i++) { __delay_ms(10); }
    
    int val=1;
    int dir=1;
    
    while(1){
        printf("radio tx 123456789012\r\n");
        //putch()
        for(uint8_t i=0;i<100;i++) {
            val=val+dir*10;
            EPWM1_LoadDutyValue((uint16_t) val);
            __delay_ms(10);
            if(val>1010)dir=-1;
            if(val<11)dir=1;
        }
        
    
   }
    
    
    return;
}
