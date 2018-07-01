#ifndef __BSP_CONFIG_H__
#define __BSP_CONFIG_H__

/**********************************BSP ERROR NUMBER********************************/
#define ARG_ChECK_ERROR		-1
#define NO_ERROR			 0


/**************************************SysTick**************************************/
#define SYSTICK_PREMMPTION_PRIO		0x0
#define SYSTICK_SUB_PRIO			0x0

/**************************************USART1***************************************/
/* Macro definition */
#define Tx_Pin	GPIO_Pin_9
#define Rx_Pin	GPIO_Pin_10

#define USART_PREMMPTION_PRIO	0x0
#define USART_SUB_PRIO			0x1
/***************************************I2C1**************************************/
#define I2C1_SCL_Pin 		GPIO_Pin_6 
#define I2C1_SDA_Pin  		GPIO_Pin_7

#define I2C1_OWNADDR		0x30
#define I2C1_CLKSPEED	200000

#define I2C_SEND_DATABUF_SIZE	256
#define I2C_RECEIVE_DATABUF_SIZE 256

/************************************TIM4*****************************************/
#define TIM4_PSC	36		/* TIM4 time base is  1¦Ìs */
#define TIM4_CNTS	1000		/* 1ms */

#define TIM4_PREMMPTION_PRIO		0x0
#define TIM4_SUB_PRIO				0x2
/***********************************Ultrasonic************************************/
#define ULTRASONIC_TRIG_Pin			GPIO_Pin_5
#define ULTRASONIC_ECHO_Pin			GPIO_Pin_6
#define ULTRASONIC_EXTI_LINE_ECHO	EXTI_Line6
#define GPIO_PINSOURCE				GPIO_PinSource6

#define ULTRASONIC_PREMMPTION_PRIO		0x1
#define ULTRASONIC_SUB_PRIO				0x2

#define ULTRASONIC_MODULE_INTERVAL					25	/* The distance between transmitting port and receiving port (mm)*/
#define DISTANCE_DATABUF_SIZE						10


/*********************************************************************************/





#endif







































