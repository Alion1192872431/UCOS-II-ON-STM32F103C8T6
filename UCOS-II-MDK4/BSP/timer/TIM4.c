/********************************************************************************************
 * File Name:TIM4.c
 * Description: This file is designed to do some work about timer4
 * Author:Alion
 * Date:2018.5.24
 ********************************************************************************************/

/* Header Files */
#include "..\..\APP\includes.h"
 
/* Global Variables */




/*
 * Function Name:TIM4_RCC_Cmd
 * Description:To enable or disable time clock related to timer4
 * Input:ENABLE or DISABLE
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.24
 */
static void TIM4_RCC_Cmd(FunctionalState NewState)
{ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, NewState);
}



/*
 * Function Name:TIM4_NVIC_Init
 * Description:To configure NVIC related to timer4
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.24
 */
static void TIM4_NVIC_Init(void)
{
	/* To config NVIC channel for USART1 */
	NVIC_InitTypeDef TIM4_NVIC_InitStructure;
	
	TIM4_NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    TIM4_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= TIM4_PREMMPTION_PRIO;
    TIM4_NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM4_SUB_PRIO;	 
    TIM4_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		 
    NVIC_Init(&TIM4_NVIC_InitStructure); 
}

/*
 * Function Name:TIM4_Mode_Config
 * Description:To configure the parameters directly related to suart1
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.24
 */
static void TIM4_Mode_Config(void)
{
	/* To config basic parameters for timer4 */
	TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;  

	TIM4_TimeBaseStructure.TIM_Prescaler = TIM4_PSC-1;
	TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM4_TimeBaseStructure.TIM_Period = TIM4_CNTS-1;
	TIM4_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4, &TIM4_TimeBaseStructure);

	/* Clear IT pending bit  */
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
	/* Enable timer update interrupt */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	/* Disable timer */
	TIM_Cmd(TIM4, DISABLE);
}



/*******************************************************************************
* Function Name: TIM4_Init
* Description: Initialize timer4
* Input: None
* Output: None
* Return: None
* Author:Alion
* Date:2018.5.24
*******************************************************************************/
void TIM4_Init(void)
{
	TIM4_RCC_Cmd(ENABLE);
	TIM4_NVIC_Init();
	TIM4_Mode_Config();
}

  

