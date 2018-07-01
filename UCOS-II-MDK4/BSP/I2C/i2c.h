#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f10x.h"

/* Type Definition */
typedef enum
{
	I2C_WRITE = 0,
	I2C_READ = 1,
}I2C_RDWR_Type;

void I2C1_Init(void);
int8_t I2C_RW(uint8_t *DataBuf,uint8_t ByteQuantity,uint8_t RegAddr,uint8_t SlaveAddr,I2C_RDWR_Type ControlByte);


#endif

























