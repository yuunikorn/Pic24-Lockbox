/* Library of definitions and functions for I2C using PIC24 peripheral
* CT 2/24/17
 */
#include <p24hxxxx.h> 
#include "stdint.h"
//Macros from textbook library for setting read and write bits
#define I2C_WADDR(x) (x & 0xFE) //clear R/W bit of I2C addr
#define I2C_RADDR(x) (x | 0x01) //set R/W bit of I2C addr
//Lower level function declarations
void InitI2C(void);
void StartI2C(void);
void RStartI2C(void);
void StopI2C(void);
void PutI2C(uint8_t val);
uint8_t PutNoAckI2C(uint8_t val);
uint8_t GetI2C(uint8_t ackval);
//Higher level function declarations
void WriteI2C(uint8_t address, uint8_t data_out);
void Write2I2C(uint8_t address, uint8_t data1, uint8_t data2);
void WriteNI2C(uint8_t addr, uint8_t* data, uint16_t cnt);
void ReadI2C(uint8_t addr, uint8_t* data);
void Read2I2C(uint8_t addr, uint8_t* data1, uint8_t* data2);
void ReadNI2C(uint8_t addr, uint8_t* data, uint16_t cnt);




