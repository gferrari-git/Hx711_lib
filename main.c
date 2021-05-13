/*
 * Nombre del archivo:   main.c
 *
*/

#include <xc.h>
#include <stdint.h>
#include "user.h"
#include "system.h"
#include "onewire.h"
#include "ds18b20.h"

#define READ_ADDR
/* ------------------------ Implementación de funciones --------------------- */
#ifdef READ_ADDR
void main(void) {// Función principal
    initApp();
    onewireInit();
    uartConfig();
    
    while(1){
    printSingleAddress();
    __delay_ms(500);
    }
}
#endif

#ifdef READ_TEMP
void main(void) {               // Función principal
    unsigned char addr[]={0x28,0x88,0x77,0xAA,0xEF,0x00,0x00,0x43};
    float Temp;
    initApp();
    onewireInit();
    uartConfig();
    
    while(1){
        broadcastConvert();
        __delay_ms(200);
        Temp= getTemperature(addr);
        printf("%.2f\r\n",Temp);
        __delay_ms(500);
    }
}
#endif



/* ------------------------ Fin de archivo ---------------------------------- */
