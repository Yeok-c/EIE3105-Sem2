#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "pinout.h"

int ledFlag = 0;

void INDICATOR_LED_init(){	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void TIM2_init(void){
	//Timer 2 set up 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure; 
	timerInitStructure.TIM_Prescaler = 18000;  //1/(72Mhz/18000)=0.25ms
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//timerInitStructure.TIM_Period = 4000;  //0.25ms*4000 = 1s
	//timerInitStructure.TIM_Period = 400;  //0.25ms*400 = 0.1s
	timerInitStructure.TIM_Period = 4;  //0.25ms*40 = 0.001s
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	
	//Enable update event for Timer2
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);	
}


void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		ledFlag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
