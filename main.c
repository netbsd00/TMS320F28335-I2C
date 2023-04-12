/*
 * main.c
 *
 *  Created on: 2023. 4. 12.
 *      Author: hwyi
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "i2c/i2c.h"

void main(void)
{
    Uint16 Error;
    Uint16 i;
    struct I2CMessage i2c_msg;

    InitSysCtrl();
    InitI2CGpio();

    //
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    I2C_Init();
    EINT;

    i = 0xFFFF;
    for(;;)
    {
        i2c_msg.SlaveAddress = 0xC0;    // address of MCP4725 DAC
        i2c_msg.NumOfSendBytes = 3;
        i2c_msg.SendBuffer[0] = 0x40;
        i2c_msg.SendBuffer[1] = i >> 8;
        i2c_msg.SendBuffer[2] = i & 0x00FF;
        I2C_Write(&i2c_msg);
        i -= 0x0020;
        DELAY_US(3000);

        i2c_msg.NumOfSendBytes = 0;
        i2c_msg.NumOfRecvBytes = 5;
        I2C_Read(&i2c_msg);
    }   // end of for(;;)
}   // end of main

//
// End of File
//




