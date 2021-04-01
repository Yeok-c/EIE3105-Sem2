#include "stm32f10x.h"                  // Device header
int state_exti = 0;
unsigned int CNT_L = 0;
unsigned int CNT_R = 0;
// PA0 - BUTT_0
// PC13- BUTT_1
// PA1 - CNT_L 
// PB7 - CNT_R

// Right Wheel Counter EXTI
void EXTI1_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line1) != RESET){ //Right Wheel
		EXTI_ClearITPendingBit(EXTI_Line1);
		CNT_R++;
	}
}

// Left Wheel Counter & On-Board Button EXTI
void EXTI9_5_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line7) != RESET){ //Left Wheel
		EXTI_ClearITPendingBit(EXTI_Line7);
		CNT_L++;
	}
	/*
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){ //Button A0
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	*/
}

// Right Wheel Counter Init PA1
void Right_Wheel_Cnt_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	// PA1 Init for Right Wheel Counter
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// EXTI Configuration
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// Enable Interrupt
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_EnableIRQ(EXTI1_IRQn);
}

// Left Wheel Counter Init PB7
void Left_Wheel_Cnt_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	// PB7 Init for Left Wheel Counter
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	// EXTI Configuration
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line7;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// Enable Interrupt
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

// On-board button init PB8
void Button_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		// EXTI Configuration
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_EnableIRQ(EXTI0_IRQn);
}

void exti_init(){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//Init port A Pins
	GPIO_InitTypeDef GPIO_InitStr;
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStr);
	
	//Init port B Pins
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStr);
	
	//Init port C Pins
	GPIO_InitStr.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStr.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStr.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStr);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);	
	
	EXTI_InitTypeDef EXTI_InitStr;
	EXTI_InitStr.EXTI_Line = EXTI_Line0;
	EXTI_InitStr.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStr.EXTI_LineCmd = ENABLE;
	EXTI_InitStr.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStr);
	
	NVIC_InitTypeDef NVIC_InitStr;
	NVIC_InitStr.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStr.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStr.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_Init(&NVIC_InitStr);
}

void EXTI0_IRQHandler(){
	if (EXTI_GetITStatus(EXTI_Line0) != RESET){
		state_exti ^= 1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

