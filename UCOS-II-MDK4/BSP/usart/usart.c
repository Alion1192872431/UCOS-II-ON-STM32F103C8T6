
/********************************************************************************************
 * File Name:usart.c
 * Description: This file is designed to do some work about usart1
 * Author:Alion
 * Date:2018.5.15
 ********************************************************************************************/

/* Header Files */
#include "..\..\APP\includes.h"
 
/* Global Variables */
uint8_t usart1_buf[256];		/* Receiving data buffer for usart1 */

#ifdef __GNUC__ 
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf 
     set to 'Yes') calls __io_putchar() */ 
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch) 
#else 
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f) 
#endif /* __GNUC__ */ 

/*******************************************************************************
* Function Name: PUTCHAR_PROTOTYPE
* Description: Retargets the C library printf function to the USART.
* Input: None
* Output: None
* Return: None
* Author:Alion
* Date:2018.5.15
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USART1, (uint16_t)ch);
  
   return ch;
}


/*
 * Function Name:RCC_USART1_Cmd
 * Description:To enable or disable time clock related to usart1
 * Input:ENABLE or DISABLE
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.15
 */
static void USART1_RCC_Cmd(FunctionalState NewState)
{ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, NewState);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, NewState);
}

/*
 * Function Name:USART1_NVIC_Init
 * Description:To configure NVIC related to usart1
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.15
 */
static void USART1_NVIC_Init(void)
{
	/* To config NVIC channel for USART1 */
	NVIC_InitTypeDef USART1_NVIC_InitStructure;
	
	USART1_NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    USART1_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= USART_PREMMPTION_PRIO;
    USART1_NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART_SUB_PRIO;	 
    USART1_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		 
    NVIC_Init(&USART1_NVIC_InitStructure); 
}





/*
 * Function Name:USART1_GPIO_Init
 * Description:To configure gpios related to usart1
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.15
 */
static void USART1_GPIO_Init(void)
{
	/* To config gpios for USART1 */
	GPIO_InitTypeDef USART1_Tx_InitStructure;
	GPIO_InitTypeDef USART1_Rx_InitStructure;

	USART1_Tx_InitStructure.GPIO_Pin = Tx_Pin;
	USART1_Tx_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	USART1_Tx_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	USART1_Rx_InitStructure.GPIO_Pin = Rx_Pin;
	USART1_Rx_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	GPIO_Init(GPIOA, &USART1_Tx_InitStructure);
	GPIO_Init(GPIOA, &USART1_Rx_InitStructure);
}

/*
 * Function Name:USART1_Mode_Config
 * Description:To configure the parameters directly related to suart1
 * Input:Baud rate
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.15
 */
static void USART1_Mode_Config(uint32_t BaudRate)
{
	/* To config basic parameters for USART1 */
	USART_InitTypeDef USART1_InitStructure;
	
	USART1_InitStructure.USART_BaudRate = BaudRate;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_Parity = USART_Parity_No ;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART1_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
	USART_Cmd(USART1, ENABLE);
}


/*
 * Function Name:USART1_Init
 * Description:To initialize usart1 
 * Input:Baud Rate
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.25
 */
void USART1_Init(uint32_t BaudRate)
{   
	USART1_RCC_Cmd(ENABLE);
	USART1_GPIO_Init(); 
	USART1_NVIC_Init(); 
	USART1_Mode_Config(BaudRate);			  	
}

