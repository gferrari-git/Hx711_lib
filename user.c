/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/
/** @file
 * @brief	Contiene las definiciones de user
 * 
 * Aqui se encuentra la implementaci�n de users, los defines, macros, 
 * datos internos y externos, declaraciones y definiciones de funciones
 * internas y definiciones de funciones externas
 */

/*==================[inlcusiones]============================================*/
#include <xc.h>         /* Archivo de Encabezados General XC8 */
#include <stdint.h>     /* para las definiciones de uint8_t por ej.*/

#include "system.h"     /* Funciones/Parametros System, como conf de osc/perif */
#include "user.h"       /* Funciones/Parametros User, como InitApp */

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/
//char datoUser; //Ejemplo
/* This global variable holds the tick count */
volatile tick_t tickCounter;/* This global variable holds the tick count */


//volatile tick_t tickRateMS;
//volatile sAPI_FuncPtr_t tickHookFunction = sAPI_NullFuncPtr;

/*==================[declaraciones de funciones internas]====================*/
//static void funcUser(void); //Ejemplo

/*==================[definiciones de funciones internas]=====================*/
//static void funcUser(void){ //Ejemplo

//}; 
/*==================[definiciones de funciones externas]=====================*/
/**
 * @brief	Inicializa Ports, Perif�ricos e Interrupciones
 * @return	Nada
 */
void initApp(void) {
    /* TODO: Inicializa Ports/Perif�ricos/Interrupciones */

    /* Configura funciones anal�gicas y direcci�n de los Ports */
    ANSEL = 0b00001111;
    ANSELH = 0;

    //Configura el TIMER0
    T0CS = 0;       //Internal instruction cycle clock (FOSC/4)
    PSA = 0;        //Prescaler Assignment bit
    
        // 1 = Prescaler is assigned to the WDT
                    // 0 = Prescaler is assigned to the Timer0 module
    OPTION_REGbits.PS = 1;  //PS<2:0>: Prescaler Rate Select bits
                            /* 1 : 2
                             * 1 : 4 <-
                             * 1 : 8
                             * 1 : 16
                             * 1 : 32
                             * 1 : 64
                             * 1 : 128
                             * 1 : 256 */
    
    //Configura el TIMER1
    TMR1CS = 0;     //Timer1 Clock Source Select bit: 0 = Internal clock (FOSC/4)
    T1CKPS0 = 0;    //T1CKPS<1:0>:Timer1 Input Clock Prescale Select bits
    T1CKPS1 = 1;    /* 11 = 1:8 Prescale Value
                     * 10 = 1:4 Prescale Value<----
                     * 01 = 1:2 Prescale Value
                     * 00 = 1:1 Prescale Value*/
    //TMR1ON = 1;     //Enables Timer1
    
    //Configura el modulo CCP1 como Comparador
	//CCP1CON = 0b00001011;	//Compare mode, trigger special event (CCPxIF bit is set; CCP1 resets TMR1)
    //CCPR1 = 1000;           //Compara con 1000 uSegs
    
    TMR0IE = 1;
    PEIE = 1;
    GIE = 0;  
}

/**
 * @brief	Configura e inicializa la UART
 * @return	Nada
 * @note    Las opciones se modifican en el c�digo, por ejemplo la 
 * constante BAUDRATE
 * @sa      BAUDRATE    
 */
void uartConfig(void) {
#if((SYS_FREQ - BAUDRATE*16L)/(BAUDRATE*16L)) > 255 //Si necesita High Baud Rate
#if	(SYS_FREQ - BAUDRATE*64L)/(BAUDRATE*64L) > 255
#error Baud Rate demasiado Bajo
#endif
    SPBRG = (SYS_FREQ - BAUDRATE * 64L) / (BAUDRATE * 64L); //SPBRG para Low Baud Rate
    BRGH = 0;   //Selecciona Low Baud Rate
#else
    SPBRG = (SYS_FREQ - BAUDRATE * 16L) / (BAUDRATE * 16L); //SPBRG para High Baud Rate
    BRGH = 1;   //Selecciona High Baud Rate
#endif
    SYNC = 0;   //Selecciona Modo Asincronico
    SPEN = 1;   //Habilita Serial Port
    TRISC7 = 1;
    TRISC6 = 1;
    CREN = 1;   //Habilita Continuous Reception
    TXEN = 1;   //Habilita Transmision
}

/**
 * @brief	Envia un byte en la salida stdout
 * @param	data Byte o dato a transmitir
 * @return	Nada
 * @note    Define la salida estandar para la librer�a stdio
 */
void putch(char data) {
    while (!TXIF)
        continue;
    TXREG = data;
}

/**
 * @brief       Envia un byte en forma boqueante
 * @param[in]   value Valor a transmitir 
 * @return      Nada
 */
void uartWriteByte( uint8_t value ){
    // Wait for space in FIFO (blocking)
    while (!TXIF)
        continue;
    // Send byte
    TXREG = value;
}

/**
 * @brief	Toma un byte de la entrada stdin
 * @return	El byte recibido
 * @note    Define la entrada estandar para la librer�a stdio
 */
char getch(void) {
    while (!RCIF)
        continue;
    return RCREG;
}

/**
 * @brief       Toma un byte de la FIFO de recepci�n, chequea primero si hay un dato disponible
 * @param[out]  receivedByte Apunta al destino para el byte recibido
 * @return      1 si hay un byte recibido, 0 si no hay dato disponible 
 */
 uint8_t uartReadByte( uint8_t* receivedByte ){
   if ( RCIF ){
      *receivedByte = RCREG;
      return 1;
   }
   return 0;
}

/*
 * @brief	Toma un byte de la entrada stdin con eco
 * @return  El byte recibido
 * @note
 */
char getche(void) {
    char data;
    while (!RCIF)
        continue;
    data = RCREG;
    while (!TXIF)
        continue;
    TXREG = data;
    return data;
}

void displayWrite( uint8_t value ){
    const uint8_t tabla7seg[10] = {
        0b00111111, //0	
        0b00000110, //1	
        0b01011011, //2	
        0b01001111, //3	
        0b01100110, //4	
        0b01101101, //5	
        0b01111101, //6	
        0b00000111, //7	
        0b01111111, //8	
        0b01100111  //9	     
    };
    PORTD = tabla7seg[value] ;  
}

/* Lee Tick Counter */
tick_t tickRead( void ) {
    tick_t aux;
    GIE = 0;                        //Inicia Zona critica
    aux = tickCounter;              //  Operaciones atomicas
    GIE = 1;                        //Finaliza la zona critica
    return aux;
}

/* Escribe Tick Counter */
void tickWrite( tick_t ticks ) {
    GIE = 0;                        //Inicia Zona critica
    tickCounter = ticks;            //  Operaciones atomicas
    GIE = 1;                        //Finaliza la zona critica
}

void my_ftoa(float* fvalue, char* farray, uint8_t decs){
    int ent;
    char i;
    uint8_t auxc;
	float aux;

	ent = (int) (*fvalue);
	itoa(ent,farray,10);
	aux = *fvalue - (float)ent;
	if(aux<0.0f){
		aux = -aux;
	}
	i=0;
	while(farray[i]!='\0'){
		i++;
	}
	farray[i]='.';
	i++;
	for(;decs>0;decs--){
	auxc=
	aux=aux*10;
	auxc= (uint8_t)aux;
	aux =aux-(float)auxc;
	farray[i]=auxc+'0';
	i++;
	}
	farray[i]='\0';
}

// Function to swap two numbers
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}
 
// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int basen)
{
    // invalid input
    if (basen < 2 || basen > 32) {
        return buffer;
    }
    int n;
    // consider the absolute value of the number
    if(value<0) n=value*-1;
    else        n=value;
 
    int i = 0;
    while (n)
    {
        int r = n % basen;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / basen;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the basen is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other basen, value is always considered unsigned
    if (value < 0 && basen == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}
/*==================[fin del archivo]========================================*/
