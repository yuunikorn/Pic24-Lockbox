/*
 * File:   main.c
 * Author: yuchangou
 *
 * Created on March 2, 2017, 12:26 AM
 */

#include "p24hxxxx.h"           //generic header for PIC24H family */
#include "xc.h"
#include "stdint.h"             //header for standard integer types
#include "stdio.h"              
#include "ser.h"                //includes the ser.h header file
#include "I2C_per.h"  
#include "lcd_SF.h"


#define FCY 3685000L            //needed for X16 compiler library delays in libpic30.h
#include <libpic30.h>           //includes definitions for __delay_ms and __delay_us
#include <p24HJ128GP502.h>


#pragma config FNOSC=FRC 
#pragma config FCKSM=CSDCMD 
#pragma config OSCIOFNC=ON 
#pragma config POSCMD=NONE 
#pragma config FWDTEN = OFF

#define SCL LATBbits.LATB8 //pin for eeprom 
#define SDA LATBbits.LATB9 //pin for eeprom 

#define I2CAddress 0b1010000 // address of eeprom
#define pageAddress 4 // size of array 
#define passwordLength 4 // password length

#define pA LATBbits.LATB12 // config pin fo r motor controller 
#define pB LATBbits.LATB11 // config pin for motor controller 
#define EN_12 LATBbits.LATB10 // config pin for motor controller 



#define row1 LATAbits.LATA1 //row 1 of number pad ()
#define row2 LATAbits.LATA2 //row 2 of number pad ()
#define row3 LATBbits.LATB3 //row 3 of number pad ()
#define row4 LATBbits.LATB1 //row 4 of number pad ()

#define col1 PORTBbits.RB0 //column 1 of number pad ()
#define col2 PORTAbits.RA0 //column 2 of number pad ()
#define col3 PORTBbits.RB2 //column 3 of number pad ()



#define button PORTAbits.RA3 //define pin for button 
#define speaker LATAbits.LATA4 // define pin for speaker

char DataRead[pageAddress];             //read the size of page
char DataWrite[pageAddress + 2]; 
char initalPass[4];
char userPass[4];
void configPin(void){
   TRISAbits.TRISA0 = 1; // set A0 as input
   TRISAbits.TRISA1 = 0; // set A1 as output
   TRISAbits.TRISA2 = 1; // set A2 as input
   TRISAbits.TRISA3 = 1; // set A3 as input
   TRISAbits.TRISA4 = 0; // set A4 as output
   
   TRISBbits.TRISB0 = 1; // set B0 as input
   TRISBbits.TRISB1 = 0; // set B1 as output
   TRISBbits.TRISB2 = 0; // set B2 as output
   TRISBbits.TRISB3 = 1; // set B3 as output
   
   CNPU1bits.CN2PUE=1;
   CNPU1bits.CN4PUE=1;
   CNPU1bits.CN6PUE=1;
   
}

char READ_NUM(void){
    row1=0; row2=1; row3=1; row4=1;
    if(col1==0){
        __delay_ms(250);
        while(col1=0);
        return '1';
    }
    
    if(col2==0){
        __delay_ms(250);
        while(col1=0);
        return '2';
    }
    
    if(col3==0){
        __delay_ms(250);
        while(col1=0);
        return '3';
    }
    
    row1=1; 
    row2=0; 
    row3=1; 
    row4=1;
    
        if(col1==0){
        __delay_ms(250);
        while(col1=0);
        return '4';
    }
    
    if(col2==0){
        __delay_ms(250);
        while(col1=0);
        return '5';
    }
    
    if(col3==0){
        __delay_ms(250);
        while(col1=0);
        return '6';
    }
    
    row1=1; 
    row2=1; 
    row3=0; 
    row4=1;
    if(col1==0){
        __delay_ms(250);
        while(col1=0);
        return '7';
    }
    
    if(col2==0){
        __delay_ms(250);
        while(col1=0);
        return '8';
    }
    
    if(col3==0){
        __delay_ms(250);
        while(col1=0);
        return '9';
    }
    
    row1=1; 
    row2=1; 
    row3=1; 
    row4=0;
    if(col1==0){
        __delay_ms(250);
        while(col1=0);
        return '*';
    }
    
    if(col2==0){
        __delay_ms(250);
        while(col1=0);
        return '0';
    }
    
    if(col3==0){
        __delay_ms(250);
        while(col1=0);
        return '#';
    }
    
    return'n';
}

char GetKey(void){
    char key='n';
    while(key=='n'){
        key= READ_NUM();
    }
    return key;
}

void defaultPass(void){
    DataWrite[0]=0x00;
    DataWrite[1]=0x00;
    initalPass[0]='1';
    initalPass[1]='2';
    initalPass[2]='3';
    initalPass[3]='4';
    
    
    int j=0;
    int i=2;
        for(i=2; i<=pageAddress+2; i++){
        DataWrite[i]=initalPass[j];
        j++;
    }
}



int main(void) {
    Init_LCD();
    ClearScreen();
    configPin();
    InitI2C();
    char Key='n';
    
    DataWrite[0]=0x00;
    DataWrite[1]=0x00;
    
    EN_12=1;
    
    while(1){
        int i=0;
        for(i=0; i<=pageAddress; i++){
            Key=GetKey();
            userPass[i]=Key;
            ClearScreen();
            WriteCMD(0b10000000);
            WriteData(userPass[i]);
        }

    }
    
    
}
