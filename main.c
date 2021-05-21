/*
 * Nombre del archivo:   main.c
 *
*/

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "user.h"
#include "system.h"
#include "hx711.h"


void main(void){
    int32_t value,acum;
    float weight;
    initApp();
    uartConfig();
    hx711Init();
    while(1){
        acum=0;
        for(uint8_t i=0;i<20;i++){
            value=hx711Read();
            acum+=value;
            __delay_ms(100);
        }
        acum/=20;
        weight=M*acum+B;
        printf("%.2f\r\n",weight);  
        __delay_ms(500);
                
    }
    
}

/* ------------------------ Fin de archivo ---------------------------------- */
