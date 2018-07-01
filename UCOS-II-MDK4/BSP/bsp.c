#include "..\App\includes.h"

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void RCC_Configuration(void)
{
	uint32_t TimeOutCnt = 0;
	uint8_t  HSEReadyFlag = 0;
	
	/* Deinitialize RCC, disable PLL */ 
	RCC_DeInit();
	
	/* Select crystal source*/
	RCC_HSEConfig(RCC_HSE_ON);
	
	/* Wait for HSE startup */ 
	do
	{ 
	   HSEReadyFlag = RCC_WaitForHSEStartUp();
	   TimeOutCnt++;
	}while((HSEReadyFlag == ERROR) && (TimeOutCnt < 0xFFFFFF));
	
	if(RCC_WaitForHSEStartUp() == ERROR)
	{
		
		while(1);		
	}
	else
	{ 
		/* Enable Prefetch Buffer */
    	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    	/* Flash 2 wait state */
    	FLASH_SetLatency(FLASH_Latency_2);
		
		/* Select PLL clock source and Multiplier factor*/ 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		
		/* Configure AHB¡¢APB1¡¢APB2 prescaler factor */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);
				
		/* Wait for PLL to be stable */
		TimeOutCnt = 0;
		while((RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) &&  (TimeOutCnt < 0xFFFFFF))
		{
			TimeOutCnt++;
		}
			
		if(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
			while(1);
		}
		else
		{
			/* Switch system clock source */
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
			
			/* Wait switching to be stable */ 
			TimeOutCnt = 0;
			while((RCC_GetSYSCLKSource() != 0x08) && (TimeOutCnt < 0xFFFFFF))
			{	
				TimeOutCnt++;
			}
				
			if(RCC_GetSYSCLKSource() != 0x08)
			{
				while(1);
			}
		}	
   }
} 

 

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#if defined (VECT_TAB_RAM)
  	/* Set the Vector Table base location at 0x20000000 */ 
  	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#elif defined(VECT_TAB_FLASH_IAP)
  	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);
#else  /* VECT_TAB_FLASH  */
  	/* Set the Vector Table base location at 0x08000000 */ 
  	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif 

  	/* Configure the NVIC Preemption Priority Bits */  
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
}

void BSP_Init(void)
{
  	/* System Clocks Configuration */
  	RCC_Configuration();   
 
  	/* NVIC configuration */
  	NVIC_Configuration();
	
  	/* USART1 Initialization  */
  	USART1_Init(9600);
	/* I2C1 Initialization */
	I2C1_Init();
	/* TIM4 Initialization */
	TIM4_Init();
	/* Ultrasonic Initialization */
	Ultrasonic_Init(); 
}

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;


    freq = BSP_CPU_ClkFreq();
    return (freq);
}


#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

