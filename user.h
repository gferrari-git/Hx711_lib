/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/
/** @file
 * @brief	Contiene macros y definiciones de user
 * 
 * Aqui se definen macros, tipos de datos y declaraci�n de funciones y datos 
 * externos de user
 */
#ifndef USER_H
#define USER_H
/*==================[inclusiones]============================================*/
#include <stdint.h>     /* para las definiciones de uint8_t por ej.*/

/*==================[macros]=================================================*/
/* User Level #define Macros                                                 */
/* TODO: Los par�metros de user.c pueden ponerse aqui */



#define	BAUDRATE        9600
#define	TICS_X_MS       1

/*==================[tipos de datos declarados por el usuario]===============*/
typedef uint16_t tick_t; // Define tipo de dato para Tick 

/*==================[declaraciones de datos externos]========================*/
//extern char datoUser; //Ejemplo
extern volatile tick_t tickCounter; //Contador de Tick

/*==================[declaraciones de funciones externas]====================*/
/* TODO: Los prototipos de User level (ej. InitApp) van aqui */
void initApp(void);     /* Inicializaci�n de I/O y eripheral Initialization */
void uartConfig(void);  /* Inicializaci�n de UART */
void putch(char data);  /* pone un byte e stdout */
char getch(void);       /* toma un byte e stdin  */
char getche(void);      /* toma un byte e stdin con eco */
void uartWriteByte( uint8_t value );        /* Envia un byte en forma boqueante */
uint8_t uartReadByte( uint8_t* receivedByte );  /* Toma un byte de la UART */
tick_t tickRead( void );        /* Lee Tick Counter */
void tickWrite( tick_t ticks ); /* Escribe Tick Counter */
void my_ftoa(float* fvalue, char* farray, uint8_t decs);
char* itoa(int value, char* result, int basen);
void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);

/*==================[fin del archivo]========================================*/
#endif// USER_H
