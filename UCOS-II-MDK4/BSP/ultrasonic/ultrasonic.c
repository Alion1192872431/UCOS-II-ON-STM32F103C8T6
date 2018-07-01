/********************************************************************************************
 * File Name:TIM4.c
 * Description: This file is designed to do some work about timer4
 * Author:Alion
 * Date:2018.5.24
 ********************************************************************************************/
 
 /* Header Files */
#include "..\..\APP\includes.h"
 
/* Global Variables */ 
uint32_t time; 
uint8_t msHcCount = 0;

/*
 * Function Name:Ultrasonic_RCC_Cmd
 * Description:To enable or disable time clock related to ultrasonic
 * Input:ENABLE or DISABLE
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.24
 */
static void Ultrasonic_RCC_Cmd(FunctionalState NewState)
{ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, NewState);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, NewState);
}


/*
 * Function Name:Ultrasonic_NVIC_Init
 * Description:To configure NVIC related to ultrasonic
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.15
 */
static void Ultrasonic_NVIC_Init(void)
{
	/* To config NVIC channel for ultrasonic */
	NVIC_InitTypeDef Ultrasonic_NVIC_InitStructure;
	
    Ultrasonic_NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    Ultrasonic_NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ULTRASONIC_PREMMPTION_PRIO;
    Ultrasonic_NVIC_InitStructure.NVIC_IRQChannelSubPriority = ULTRASONIC_SUB_PRIO;
    Ultrasonic_NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
    NVIC_Init(&Ultrasonic_NVIC_InitStructure);
}


/*
 * Function Name:Ultrasonic_GPIO_Init
 * Description:To configure gpios related to ultrasonic
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.15
 */
static void Ultrasonic_GPIO_Init(void)
{
	/* To configure gpios for ultrasonic */
	GPIO_InitTypeDef Ultrasonic_TRIG_InitStructure;
	GPIO_InitTypeDef Ultrasonic_ECHO_InitStructure;

	Ultrasonic_TRIG_InitStructure.GPIO_Pin = ULTRASONIC_TRIG_Pin;
	Ultrasonic_TRIG_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* Set as general output */
	Ultrasonic_TRIG_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	Ultrasonic_ECHO_InitStructure.GPIO_Pin = ULTRASONIC_ECHO_Pin;
	Ultrasonic_ECHO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	/* Set as input pull-down */

	GPIO_Init(GPIOB, &Ultrasonic_TRIG_InitStructure);
	GPIO_Init(GPIOB, &Ultrasonic_ECHO_InitStructure);

	/* Pull down trig pin at first */
	GPIO_ResetBits(GPIOB,ULTRASONIC_TRIG_Pin);
	GPIO_ResetBits(GPIOB,ULTRASONIC_ECHO_Pin);
}

/*
 * Function Name:Ultrasonic_EXTI_Config
 * Description:To configure the EXTI
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.15
 */
static void Ultrasonic_EXTI_Config(void)
{
	/* Configure EXTI for Ultrasonic's ECHO pin */ 
	EXTI_InitTypeDef Ultrasonic_EXTI_ECHO_InitStructure;
 
	Ultrasonic_EXTI_ECHO_InitStructure.EXTI_Line = ULTRASONIC_EXTI_LINE_ECHO;
	Ultrasonic_EXTI_ECHO_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	/* Interrupt mode */
	Ultrasonic_EXTI_ECHO_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;/* Falling edge trigger */
	Ultrasonic_EXTI_ECHO_InitStructure.EXTI_LineCmd = ENABLE;

	/* Set ECHO pin as EXTI line */ 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PINSOURCE);
	/* Clear IT pending bit */
	EXTI_ClearITPendingBit(ULTRASONIC_EXTI_LINE_ECHO);
	/* Configure EXTI line */ 	
	EXTI_Init(&Ultrasonic_EXTI_ECHO_InitStructure);	
}

/*
 * Function Name:Ultrasonic_Init
 * Description:To initialize ultrasonic 
 * Input:Baud Rate
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.25
 */
void Ultrasonic_Init(void)
{   
	Ultrasonic_RCC_Cmd(ENABLE);
	Ultrasonic_GPIO_Init(); 
	Ultrasonic_EXTI_Config();
	Ultrasonic_NVIC_Init();
}

/*
 * Function Name:Ultrasonic_delayus
 * Description:To delay some time(incorrect) 
 * Input:Some time to delay
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.25
 */
static void Ultrasonic_delayus(uint16_t time)
{
	volatile uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<9;j++);
}

/*
 * Function Name:Ultrasonic_Init
 * Description:To initialize ultrasonic 
 * Input:None
 * Output:None
 * Return:None
 * Author:Alion
 * Date:2018.5.25
 */
static void Ultrasonic_Transmit_Signal(void)
{
	GPIO_SetBits(GPIOB,ULTRASONIC_TRIG_Pin);	/* Pull up Triger pin */ 
	Ultrasonic_delayus(12);						 /* delay more than 10us */
	GPIO_ResetBits(GPIOB,ULTRASONIC_TRIG_Pin);	/* pull down Triger pin */ 
	Ultrasonic_delayus(50000);	
}


/*
 * Function Name:Get_distance
 * Description:To get the distance to roadblock 
 * Input:None
 * Output:None
 * Return:distance
 * Author:Alion
 * Date:2018.5.25
 */

int32_t Get_distance(void)
{
	int8_t i;
	int8_t index = -1;
	int32_t TmpDistance = 0.0, distance = 0.0;
	int32_t timelong = 0;
	float distance_data[DISTANCE_DATABUF_SIZE]={0.0}; 
	
	while(index<DISTANCE_DATABUF_SIZE-1)
	{  
		msHcCount = 0;
		Ultrasonic_Transmit_Signal(); 
		timelong = msHcCount*1000 + time; //us 
		TmpDistance = (timelong/58); 
		if((TmpDistance>0)&&(TmpDistance<350))
		{ 
			distance_data[index++] = TmpDistance;  
		}
		else if(TmpDistance>=350)
		{ 
			distance_data[index++] = 350;
		}
		else
		{
			distance_data[index++] = 0;
		} 
	} 
	
	/* Compute average  */
	for(i=0; i<DISTANCE_DATABUF_SIZE; i++)
	{
		distance += distance_data[i];
	}
	distance = distance/DISTANCE_DATABUF_SIZE;  
	
	return distance; 
}
