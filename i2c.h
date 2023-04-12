/*
 * i2c.h
 *
 *  Created on: 2023. 4. 12.
 *      Author: hwyi
 */

#ifndef I2C_I2C_H_
#define I2C_I2C_H_


struct I2CMessage
{
    Uint16 SlaveAddress; // I2C address of slave msg is intended for
    Uint16 NumOfSendBytes;
    Uint16 SendBuffer[I2C_MAX_BUFFER_SIZE];
    Uint16 NumOfRecvBytes;
    Uint16 RecvBuffer[I2C_MAX_BUFFER_SIZE];
};

void   I2C_Init(void);
Uint16 I2C_Write(struct I2CMessage *msg);
Uint16 I2C_Read(struct I2CMessage *msg);


#endif /* I2C_I2C_H_ */
