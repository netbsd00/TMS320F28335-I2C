/*
 * i2c.c
 *
 *  Created on: 2023. 4. 12.
 *      Author: hwyi
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "i2c.h"


void I2C_Init(void)
{
    //
    // Initialize I2C
    //
    I2caRegs.I2CSAR = 0x00C0;   // Slave address

#if (CPU_FRQ_150MHZ)            // Default - For 150MHz SYSCLKOUT
    //
    // Prescaler - need 7-12 Mhz on module clk (150/15 = 10MHz)
    //
    I2caRegs.I2CPSC.all = 15;
#endif
#if (CPU_FRQ_100MHZ)            // For 100 MHz SYSCLKOUT
    //
    // Prescaler - need 7-12 Mhz on module clk (100/10 = 10MHz)
    //
    I2caRegs.I2CPSC.all = 9;
#endif

    I2caRegs.I2CCLKL = 10;      // NOTE: must be non zero
    I2caRegs.I2CCLKH = 5;       // NOTE: must be non zero
    I2caRegs.I2CIER.all = 0x00; // Disable all interrupts

    //
    // Take I2C out of reset
    // Stop I2C when suspended
    //
    I2caRegs.I2CMDR.all = 0x0020;   // Enable the i2c module

    I2caRegs.I2CFFTX.all = 0x6000;  // Enable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x2040;  // Enable RXFIFO, clear RXFFINT,

    return;
}

Uint16 I2C_Write(struct I2CMessage *msg)
{
    Uint16 i;
    Uint16 status = I2C_SUCCESS;

    if(msg == 0)
    {
        status = I2C_ERROR;
    }
    else if(I2caRegs.I2CSTR.bit.BB == 1)
    {
        status = I2C_BUS_BUSY_ERROR;
    }
    else if(I2caRegs.I2CMDR.bit.STP == 1)
    {
        status = I2C_STP_NOT_READY_ERROR;
    }
    else
    {
        I2caRegs.I2CSAR = msg->SlaveAddress>>1;
        I2caRegs.I2CCNT = msg->NumOfSendBytes;

        I2caRegs.I2CMDR.bit.MST = 1;
        I2caRegs.I2CMDR.bit.TRX = 1;
        I2caRegs.I2CMDR.bit.STT = 1;
        I2caRegs.I2CMDR.bit.IRS = 1;
        I2caRegs.I2CMDR.bit.STP = 1;

        while(I2caRegs.I2CSTR.bit.XRDY == 0);

        for(i=0; i<msg->NumOfSendBytes; i++)
        {
            I2caRegs.I2CDXR = msg->SendBuffer[i];
        }

        status = I2C_SUCCESS;
    }

    return status;
}

Uint16 I2C_Read(struct I2CMessage *msg)
{
    Uint16 i;
    Uint16 status = I2C_SUCCESS;

    if(msg == 0)
    {
        status = I2C_ERROR;
    }
    else if(I2caRegs.I2CSTR.bit.BB == 1)
    {
        status = I2C_BUS_BUSY_ERROR;
    }
    else if(I2caRegs.I2CMDR.bit.STP == 1)
    {
        status = I2C_STP_NOT_READY_ERROR;
    }
    else
    {
        I2caRegs.I2CDXR = msg->SlaveAddress >> 1;
        I2caRegs.I2CCNT = msg->NumOfRecvBytes;

        I2caRegs.I2CMDR.bit.MST = 1;
        I2caRegs.I2CMDR.bit.TRX = 0;
        I2caRegs.I2CMDR.bit.STT = 1;
        I2caRegs.I2CMDR.bit.IRS = 1;

//        while(I2caRegs.I2CSTR.bit.RRDY == 0);
        while(I2caRegs.I2CFFRX.bit.RXFFST < msg->NumOfRecvBytes);

        i = 0;
        while(i < msg->NumOfRecvBytes)
        {
            msg->RecvBuffer[i++] = I2caRegs.I2CDRR;
        }

        status = I2C_SUCCESS;
    }

    return status;
}
