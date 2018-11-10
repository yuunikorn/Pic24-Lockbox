/* File:   ser.c
 * Author: Yu Chang Ou 
 * Created on February 16, 2017
 */

#include "p24hxxxx.h"  /* generic header for PIC24H family */
#include "xc.h"
#include "ser.h"


#define FCY 3685000L //define the instruction clock used in BRG calculation
#define BAUDRATE 9600 //a standard baud rate available to most serial terminals
#define BRGVAL (FCY/BAUDRATE/16)-1 //value for Baud Rate Generator register


void initUART2(void){
    U2BRG=BRGVAL;           //Set U2BRG to the BRGVAL calculated by the define statement
    U2MODEbits.UARTEN = 1;  //Enable UART2 by setting a bit in the U2MODE register
    U2STAbits.UTXEN = 1;    //Enable transmission by setting a bit in the U2STA register
}


void putU2(char c){                 //output character to screen
    while(U2STAbits.UTXBF!=0);      //Waits for transmit buffer tobe empty
        U2TXREG=c;                  //writes character to U2TXREG
}


char getU2(void){                   //reads input character from keyboard
    while(U2STAbits.URXDA!=1);      //waits for character input URXDA = 1
    return U2RXREG;                 //returns content in receive buffer
}

void putsU2(char *s){       //outputs a string of characters to screen
    while(*s !='\0'){       //while string is not ?\0? (null)
        putU2(*s);          //write next character to screen
        s++;                //move pointer to next character
    }
    putU2('\r');            //carriage return to go to next line
 }
