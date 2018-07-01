/******************************************************************************
 * File Name:i2c.c
 * Description: This file is designed to do some work about i2c
 * Author:Alion
 * Date:2018.5.26
 *****************************************************************************/
/* Header Files */
#include "..\..\APP\includes.h"

/* Global Variables */
uint8_t Write_DataBuf[I2C_SEND_DATABUF_SIZE];
uint8_t Read_DataBuf[I2C_RECEIVE_DATABUF_SIZE];


 /*
  * Function Name:I2C1_RCC_Cmd
  * Description:To ENABLE or DISABLE RCC for I2C1  
  * Input:ENABLE or DISABLE
  * Output:None
  * Return:None
  * Author:Alion
  * Date:2018.5.26
  */ 
static void I2C1_RCC_Cmd(FunctionalState NewState)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, NewState);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, NewState);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, NewState);
}

/*
 * Function Name:I2C1_GPIO_Init
 * Description:To configure gpios for I2C1 
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.26
 */ 
static void I2C1_GPIO_Init(void)
{
	/* To configure gpios for I2C */
	GPIO_InitTypeDef I2C1_SCL_InitStructure;
	GPIO_InitTypeDef I2C1_SDA_InitStructure;
	
	I2C1_SCL_InitStructure.GPIO_Pin = I2C1_SCL_Pin;
	I2C1_SCL_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	I2C1_SCL_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	I2C1_SDA_InitStructure.GPIO_Pin = I2C1_SDA_Pin;
	I2C1_SDA_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	I2C1_SDA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &I2C1_SCL_InitStructure);
	GPIO_Init(GPIOB, &I2C1_SDA_InitStructure);
	
} 
 
/*
 * Function Name:I2C1_Mode_Config
 * Description:To configure the parameters directly related to I2C1
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.26
 */ 
static void I2C1_Mode_Config(void)
{
	I2C_InitTypeDef  I2C1_InitStructure;
	
	I2C_DeInit(I2C1);
	I2C1_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C1_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C1_InitStructure.I2C_OwnAddress1 = I2C1_OWNADDR;
	I2C1_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C1_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C1_InitStructure.I2C_ClockSpeed = I2C1_CLKSPEED;
	
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C1_InitStructure);
}


/*
 * Function Name:I2C1_Init
 * Description:To initialize I2C1 
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.26
 */
void I2C1_Init(void)
{
	I2C1_RCC_Cmd(ENABLE);
	I2C1_GPIO_Init();
	I2C1_Mode_Config();
}


/*
 * Function Name:i2c_rw
 * Description:To read/write data from/to slave device through I2C  
 * Input:DataBuf----data buffer
 * ByteQuantity----byte quantity to write or read 
 * RegAddr----internal address of slave device to write to or read from
 * SlaveAddr----slave device address
 * ControlByte----control byte for read or write
 * Output:None
 * Return:Error number
 * Author:Alion
 * Date:2018.5.26
 */
int8_t I2C_RW(uint8_t *DataBuf,uint8_t ByteQuantity,uint8_t RegAddr,uint8_t SlaveAddr,I2C_RDWR_Type ControlByte)
{  
	/* Check arguments */
	if(!ByteQuantity || (ByteQuantity>=I2C_SEND_DATABUF_SIZE))
		return ARG_ChECK_ERROR;
	
	/* Generate  a start condition and switch to master mode */
	I2C_GenerateSTART(I2C1, ENABLE);
	/* Check if it's master mode, bus is busy and start condition is generated or not */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send slave device address(Write)  */
	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Transmitter);
	/* Check if address matched successfully,data register empty,master mode,bus busy, data transmitted or not */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	  
	/* Send the internal address of slave device to write to */
	I2C_SendData(I2C1, RegAddr);
	/* Check whether data transmitted successfully or not */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		
	if(ControlByte == I2C_WRITE)
	{
		do
		{
			I2C_SendData(I2C1, *DataBuf++);
			/* To make sure data transmitted and ACK successfully */
		    while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)||I2C_SR1_AF));
	    }while(ByteQuantity--);
		  
		I2C_GenerateSTOP(I2C1, ENABLE);
	}
	else
	{
		I2C_GenerateSTART(I2C1, ENABLE);
	    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	    /* Send slave device address(Read)  */
	    I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Receiver);
	    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); /* EV7 */
		
	    do
	    {
			if(ByteQuantity == 1)
		    {	
				/* To disable ACK after last data */
				I2C_AcknowledgeConfig(I2C1, DISABLE); 
	    	    I2C_GenerateSTOP(I2C1, ENABLE); 
		    }
		    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)); /* EV7 */
		    *DataBuf = I2C_ReceiveData(I2C1);
			DataBuf++;
	     }while (ByteQuantity--);
		 /* To enable ACK */ 
		 I2C_AcknowledgeConfig(I2C1, ENABLE); 	 
	}
	return NO_ERROR;
}  






















