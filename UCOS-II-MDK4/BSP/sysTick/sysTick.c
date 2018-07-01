/********************************************************************************************
 * File Name:sysTick.c
 * Description: This file is designed to do some work about sysTick
 * Author:Alion
 * Date:2018.5.12
 ********************************************************************************************/

/* Header Files */
#include "..\..\APP\includes.h"

/* Macro definition */


/* Global Variables */


/*
 * Function Name:SysTick_NVIC_Init
 * Description:To configure NVIC related to SysTick
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.12
 */
static void SysTick_NVIC_Init(void)
{
	/* To config NVIC channel for SysTick */
	NVIC_InitTypeDef SysTick_NVIC_InitStructure;
	
	SysTick_NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
    SysTick_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= SYSTICK_PREMMPTION_PRIO;
    SysTick_NVIC_InitStructure.NVIC_IRQChannelSubPriority = SYSTICK_SUB_PRIO;	 
    SysTick_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
    NVIC_Init(&SysTick_NVIC_InitStructure); 
}



/*
 * Function Name:SysTick_Mode_Config
 * Description:To configure the parameters directly related to SysTick
 * Input:Baud rate
 * Output:None
 * Return:Error number
 * Author:Alion
 * Date:2018.5.12
 */
static int8_t SysTick_Mode_Config(void)
{ 
	uint32_t reloadVal = OS_CPU_SysTickClkFreq() / OS_TICKS_PER_SEC;
	reloadVal -= 1;
	
	/* Whether the auto reload value is greater than 0xFFFFFF(24bit) or not */
	if(reloadVal > 0xFFFFFF) 
		return ARG_ChECK_ERROR;

	/* Configure auto reload register value */
	SysTick->LOAD = reloadVal;
	
	/* Configure SysTick clock as HICLK*/ 
	SysTick->CTRL |= (0x01<<2);
  
		
	/* To enable SysTick interrupt */
	SysTick->CTRL |= (0x01<<1);

	/* Clear the current value register */ 
	SysTick->VAL = 0x00;
	
	/* Enable SysTick timer */
	SysTick->CTRL |= (0x01<<0);
	
	return NO_ERROR;			
}


/*
 * Function Name:SysTick_Init
 * Description:To Initialize SysTick
 * Input:None
 * Output:None
 * Return:Error number
 * Author:Alion
 * Date:2018.5.12
 */
int8_t SysTick_Init(void)
{ 
	SysTick_NVIC_Init();
	
	if(SysTick_Mode_Config() == ARG_ChECK_ERROR)
	{
		return ARG_ChECK_ERROR;			
	}
	else
	{
		return NO_ERROR;
	}
}



























