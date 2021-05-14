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
    int32_t value;
    initApp();
    uartConfig();
    hx711Init();
    while(1){
        value=hx711Read();
        printf("%ld\r\n",value);  
        __delay_ms(500);
                
    }
    
}

/* ------------------------ Fin de archivo ---------------------------------- */
