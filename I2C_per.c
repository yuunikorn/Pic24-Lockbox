/*
 * File:   I2C_per.c
 * Author: traverc
 * Edited by: yuchangou
 * Functions for I2C module
 * Uses serial library to print error messages
 * Created on February 24, 2017, 11:59 AM
 */

#include <p24HJ128GP502.h>
#include "stdint.h"
#include "I2C_per.h"
#include "ser.h"

void InitI2C(void)
{
I2C1CONbits.I2CEN = 1; //Enable I2C module
I2C1BRG = 35; //Set for 100KHz SCL
}

void StartI2C(void){
  I2C1CONbits.SEN = 1;	/* initiate Start on SDA and SCL pins */
  while (I2C1CONbits.SEN); //wait for start to complete
}
void RStartI2C(void){
    I2C1CONbits.RSEN = 1; //initiate restart
    while (I2C1CONbits.RSEN);  //wait for restart to end
}
void StopI2C(void)
{
     I2C1CONbits.PEN = 1;	/* initiate Stop on SDA and SCL pins */
     while (I2C1CONbits.PEN);
}
void PutI2C(uint8_t val) {  //write that sends error if NACK received
    I2C1TRN = val; //write byte
    while (I2C1STATbits.TRSTAT); //wait for transmission
    if (I2C1STATbits.ACKSTAT) {
        putsU2("NACK received");
    }
}
uint8_t PutNoAckI2C(uint8_t val) { //write that returns ack bit
    I2C1TRN = val; //write byte
    while (I2C1STATbits.TRSTAT); //wait for transmission
    return(I2C1STATbits.ACKSTAT);
}
uint8_t GetI2C(uint8_t ackval) {
    uint8_t in_data;
    while (I2C1CON & 0x1F);  //wait for idle condition
    I2C1CONbits.RCEN = 1;    //enable receive
    while (!I2C1STATbits.RBF);  //wait for received data
    in_data = I2C1RCV;       //read byte
    while (I2C1CON & 0x1F);   //lower 5 bits must be 0 for idle condition
    I2C1CONbits.ACKDT = ackval;  //send ack back
    I2C1CONbits.ACKEN = 1;   //enable ack bit transmission
    while (I2C1CONbits.ACKEN); //wait for completion
    return(in_data);
}

void WriteI2C(uint8_t address, uint8_t data_out)  //write address and data
{
    StartI2C();
    PutI2C(I2C_WADDR(address));
    PutI2C(data_out);
    StopI2C();
}

void Write2I2C(uint8_t address, uint8_t data1, uint8_t data2) {  //write two bytes
    StartI2C();
    PutI2C(I2C_WADDR(address));
    PutI2C(data1);
    PutI2C(data2);
    StopI2C();
}
void WriteNI2C(uint8_t addr, uint8_t* data, uint16_t cnt){ //write N bytes
    uint16_t i; //counter for bytes
    StartI2C();
    PutI2C(I2C_WADDR(addr));
    for (i = 0; i < cnt; i++) {
        PutI2C(*data);
        data++;
    }
    StopI2C();
    }

void ReadI2C(uint8_t addr, uint8_t* data) { //read 1 byte into *data
    StartI2C();
    PutI2C (I2C_RADDR(addr));
    *data = GetI2C(1); //last ack bit from master must be NAK(1))
    StopI2C();
}

void Read2I2C(uint8_t addr, uint8_t* data1, uint8_t* data2) {
    StartI2C();
    PutI2C (I2C_RADDR(addr));
    *data1 = GetI2C(0); //first ack bit from master should be ACK
    *data2 = GetI2C(1); //last ack bit from master must be NAK
    StopI2C();
}

  void ReadNI2C(uint8_t addr, uint8_t* data, uint16_t cnt) {
    uint16_t i;
    StartI2C();
    PutI2C (I2C_RADDR(addr));
    for (i = 0; i < cnt; i++) {
        if (i != cnt-1) *data = GetI2C(0);
        else *data = GetI2C(1);
        data++;
    }
    StopI2C();
}
