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
    char peso[6];
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
        my_ftoa(&weight,peso,2);// esta funcion convierte el float weight en
                                //un string peso con 2 decimales
        printf("%s\r\n",peso);  
        __delay_ms(500);
                
    }
    
}

/* ------------------------ Fin de archivo ---------------------------------- */
