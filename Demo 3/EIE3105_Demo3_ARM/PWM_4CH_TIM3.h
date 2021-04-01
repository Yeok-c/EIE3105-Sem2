#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "pinout.h"

void TIM3_PWM_init(void) 
{
	RCC_APB2PeriphClockCmd(TIM3_CH1_PWM_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(TIM3_CH2_PWM_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(TIM3_CH3_PWM_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure I/O for Tim3 Ch1 PWM pin
	GPIO_InitStructure.GPIO_Pin = TIM3_CH1_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM3_CH1_PWM_GPIO, &GPIO_InitStructure);
	
	// Configure I/O for Tim3 Ch2 PWM pin
	GPIO_InitStructure.GPIO_Pin = TIM3_CH2_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM3_CH2_PWM_GPIO, &GPIO_InitStructure);
	
	// Configure I/O for Tim3 Ch3 PWM pin
	GPIO_InitStructure.GPIO_Pin = TIM3_CH3_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM3_CH3_PWM_GPIO, &GPIO_InitStructure);
	
	// Configure I/O for Tim3 Ch4 PWM pin
	GPIO_InitStructure.GPIO_Pin = TIM3_CH4_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM3_CH4_PWM_GPIO, &GPIO_InitStructure);
	
	//Tim3 set up 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 
  TIM_TimeBaseInitTypeDef timerInitStructure; 
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //timerInitStructure.TIM_Prescaler = 144-1;  // 1/(72Mhz/144) = 0.000 002s
  //timerInitStructure.TIM_Period = 5000-1;  //0.02us*5000 = 0.01s = 100Hz
	
	timerInitStructure.TIM_Prescaler = 720-1; //1/(72Mhz/720)=0.01ms
	//timerInitStructure.TIM_Period = 50000-1; //0.01ms*50000 = 500ms; 50,000 is also max for CCR
	timerInitStructure.TIM_Period = 500-1; //0.01ms*50000 = 5ms


  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &timerInitStructure);
  TIM_Cmd(TIM3, ENABLE);
	
	TIM_OCInitTypeDef outputChannelInit;
	//Enable Tim3 Ch1 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1000-1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//Enable Tim3 Ch2 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1000-1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &outputChannelInit);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//Enable Tim3 Ch3 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1000-1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &outputChannelInit);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	//Enable Tim3 Ch4 PWM
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 1000-1; 
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &outputChannelInit);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
}
