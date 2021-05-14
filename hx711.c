/*
 * File:   hx711.c
 * Author: g_fer
 *
 * Created on 13 de mayo de 2021, 15:08
 */


#include <xc.h>
#include "hx711.h"
#include "system.h"


void hx711Init(){
    TRIS_DOUT=1;
    TRIS_PD_CLK=0;
    PD_CLK=0;
    __delay_ms(500);
 
}
int32_t hx711Read(){
    int32_t data=0;
    while(DOUT==1);
    for(uint8_t i=0;i<24;i++){
    PD_CLK=1;
     __delay_us(1);
    if(DOUT)
    data |= 0x000001;
    if (i!=23)
    data<<=1;
   

    PD_CLK=0;
    __delay_us(1);
    }
    PD_CLK=1;
    __delay_us(1);
    PD_CLK=0;
    __delay_us(1);
    
    if(data & 0x00800000){
        data|=0xFF000000;
    }
    
    return data;
    
}