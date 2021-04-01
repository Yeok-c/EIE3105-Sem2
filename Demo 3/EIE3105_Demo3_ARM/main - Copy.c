#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "misc.h"
#include "string.h"


#define UP    0
#define DOWN  1 
#define LEFT  2
#define RIGHT 3
#define TRIANGLE 4
#define CIRCLE   5
#define CROSS    6
#define SQUARE   7


/*
	Cheah Yeok Tatt
	18078696D 
	AVR and ARM Test 2021 Sem 2
	
	Q2:
	When string ABC_ is sent - repeated in tera term
	When covered by hand - XYZ_ is shown every second
*/


//Usage
// PA0 - ADC1_IN0
// PA1 - ADC1_IN1
// PA2 - USART2 TX
// PA3 - USART2 RX
// PA4 - ADC1_IN4
// PA5 - LD2
// PA6 - Tim3 Ch1 PWM
// PA7 - Tim3 Ch2 PWM
// PB0 - Tim3 Ch3 PWM
// PB1 - Tim3 Ch4 PWM
// PC13 - B1

//B1 PC13
#define B1_RCC_GPIO        RCC_APB2Periph_GPIOC
#define B1_GPIO	           GPIOC
#define B1_GPIO_PIN        GPIO_Pin_13
#define B1_EXTI_LINE       EXTI_Line13
#define B1_GPIO_PORTSOURCE GPIO_PortSourceGPIOC
#define B1_GPIO_PINSOURCE  GPIO_PinSource13

//LD2 PA5
#define LD2_RCC_GPIO  RCC_APB2Periph_GPIOA
#define LD2_GPIO      GPIOA
#define LD2_PIN       GPIO_Pin_5

//ADC1_0 PA0
#define ADC1_0_RCC_GPIO  RCC_APB2Periph_GPIOA
#define ADC1_0_GPIO      GPIOA
#define ADC1_0_PIN       GPIO_Pin_0

//ADC1_1 PA1
#define ADC1_1_RCC_GPIO  RCC_APB2Periph_GPIOA
#define ADC1_1_GPIO      GPIOA
#define ADC1_1_PIN       GPIO_Pin_1

//ADC1_4 PA4
#define ADC1_4_RCC_GPIO  RCC_APB2Periph_GPIOA
#define ADC1_4_GPIO      GPIOA
#define ADC1_4_PIN       GPIO_Pin_4


//PWM Tim3 Ch1 PA6
#define TIM3_CH1_PWM_RCC_GPIO  RCC_APB2Periph_GPIOA
#define TIM3_CH1_PWM_GPIO      GPIOA
#define TIM3_CH1_PWM_PIN       GPIO_Pin_6
//PWM Tim3 Ch2 PA7
#define TIM3_CH2_PWM_RCC_GPIO  RCC_APB2Periph_GPIOA
#define TIM3_CH2_PWM_GPIO      GPIOA
#define TIM3_CH2_PWM_PIN       GPIO_Pin_7

//PWM Tim3 Ch3 PB0
#define TIM3_CH3_PWM_RCC_GPIO  RCC_APB2Periph_GPIOB
#define TIM3_CH3_PWM_GPIO      GPIOB
#define TIM3_CH3_PWM_PIN       GPIO_Pin_0

//PWM Tim3 Ch4 PB1
#define TIM3_CH4_PWM_RCC_GPIO  RCC_APB2Periph_GPIOB
#define TIM3_CH4_PWM_GPIO      GPIOB
#define TIM3_CH4_PWM_PIN       GPIO_Pin_1

void ADC1_1channel_init(void);
void ADC1_3channels_init(void);
void DMA1_init(void);
#define ARRAYSIZE 3
#define ADC1_DR    ((uint32_t)0x4001244C)

void INDICATOR_LED_init(){	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

volatile uint16_t ADC_values[ARRAYSIZE];
void ADC1_1channel_init(void) 
{
	ADC_InitTypeDef  ADC_InitStructure;
  //PCLK2 is the APB2 clock */
  //ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure I/O for  ADC  //no need to set default is input floating
	RCC_APB2PeriphClockCmd(ADC1_1_RCC_GPIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADC1_0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(ADC1_1_GPIO, &GPIO_InitStructure);
	
	/* Enable ADC1 clock so that we can talk to it */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* Put everything back to power-on defaults */
  ADC_DeInit(ADC1);
	
	/* ADC1 Configuration ------------------------------------------------------*/
  /* ADC1 and ADC2 operate independently */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  /* Disable the scan conversion so we do one at a time */
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;  //convert multiple channels
  /* Don't do contimuous conversions - do them on demand */
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  /* Start conversin by software, not an external trigger */
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  /* Conversions are 12 bit - put them in the lower 12 bits of the result */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  /* Say how many channels would be used by the sequencer */
  ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1,ADC_SampleTime_239Cycles5); // define regular conversion config
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 2,ADC_SampleTime_28Cycles5); 
	 /* Now do the setup */
  ADC_Init(ADC1, &ADC_InitStructure);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
	
	//enable DMA for ADC
   //ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
}

void ADC1_3channels_init(void) 
{
	ADC_InitTypeDef  ADC_InitStructure;
  //PCLK2 is the APB2 clock */
  //ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure I/O for  ADC  //no need to set default is input floating
	RCC_APB2PeriphClockCmd(ADC1_1_RCC_GPIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADC1_0_PIN | ADC1_1_PIN | ADC1_4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(ADC1_1_GPIO, &GPIO_InitStructure);
	
	/* Enable ADC1 clock so that we can talk to it */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  /* Put everything back to power-on defaults */
  ADC_DeInit(ADC1);
	
	/* ADC1 Configuration ------------------------------------------------------*/
  /* ADC1 and ADC2 operate independently */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  /* Disable the scan conversion so we do one at a time */
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;  //convert multiple channels
  /* Don't do contimuous conversions - do them on demand */
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  /* Start conversin by software, not an external trigger */
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  /* Conversions are 12 bit - put them in the lower 12 bits of the result */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  /* Say how many channels would be used by the sequencer */
  ADC_InitStructure.ADC_NbrOfChannel = 3;
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1,ADC_SampleTime_239Cycles5); // define regular conversion config
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4, 3,ADC_SampleTime_239Cycles5);
	 /* Now do the setup */
  ADC_Init(ADC1, &ADC_InitStructure);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
	
	//enable DMA for ADC
   ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
}

void DMA1_init(void) 
{
	//enable DMA1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    //create DMA structure
    DMA_InitTypeDef  DMA_InitStructure;
    //reset DMA1 channe1 to default values;
    DMA_DeInit(DMA1_Channel1);
    //channel will be used for memory to memory transfer
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    //setting normal mode (non circular)
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //medium priority
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //source and destination data size word=32bit
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //automatic memory destination increment enable.
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //source address increment disable
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //Location assigned to peripheral register will be source
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //chunk of data to be transfered
    DMA_InitStructure.DMA_BufferSize = ARRAYSIZE;
    //source and destination start addresses
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//(uint32_t)ADC1_DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_values;
    //send values to DMA registers
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    // Enable DMA1 Channel Transfer Complete interrupt
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1
    NVIC_InitTypeDef NVIC_InitStructure;
    //Enable DMA1 channel IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void TIM2_init(void){
	//Timer 2 set up 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure; 
	timerInitStructure.TIM_Prescaler = 18000;  //1/(72Mhz/18000)=0.25ms
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 4000;  //0.25ms*4000 = 1s
	//timerInitStructure.TIM_Period = 400;  //0.25ms*400 = 0.1s
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	
	//Enable update event for Timer2
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);	
}
	
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

void USART1_init(void) 
{
	//USART 1      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//USART1
	
	GPIO_InitTypeDef USART_GPIO_Init;
	//---- USART1_TX   PA9     ----------------
  USART_GPIO_Init.GPIO_Pin = GPIO_Pin_9; 
  USART_GPIO_Init.GPIO_Speed = GPIO_Speed_50MHz;
  USART_GPIO_Init.GPIO_Mode = GPIO_Mode_AF_PP;      
  GPIO_Init(GPIOA, &USART_GPIO_Init);               
   
     //-----USART1_RX   PA10     -------------
  USART_GPIO_Init.GPIO_Pin = GPIO_Pin_10;
  USART_GPIO_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_Init(GPIOA, &USART_GPIO_Init);                

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

	
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStructure; 
	
	USART_InitStructure.USART_BaudRate = 9600;
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	// Enable the USART1 TX Interrupt 
	USART_ITConfig(USART1, USART_IT_TC, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// Enable the USART1 TX Interrupt 
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// Enable the USART1 RX Interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE );
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void INDICATOR_LED_init(void);	
void TIM3_PWM_init(void);
void TIM2_init(void);
void USART2_init(void);
void USARTSend(char *pucBuffer, unsigned long ulCount);
void PWM_Update(int clk, int ch);

volatile uint32_t status = 0;
extern volatile uint16_t ADC_values[ARRAYSIZE];

char xyz[5] = {"XYZ_\t"};
char abc[5] = {"ABC_\t"};
int ledFlag = 0;
unsigned char ledState=0;
unsigned char character, newCharacter, state;

int main(void) {
	
	char buffer[50] = {'\0'};
	
	INDICATOR_LED_init();
	USART1_init();
	ADC1_3channels_init();
	TIM2_init();
	TIM3_PWM_init();
	DMA1_init();
	
	//Enable DMA1 Channel transfer
	// DMA_Cmd(DMA1_Channel1, ENABLE);
	// start conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// start conversion (will be endless as we are in continuous mode)
	
	while(1) {
		//while(!status); //for DMA
				
		if(ledFlag == 1){				
			switch(ledState){
					case 0: 
						GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
						ledState =1;
						break;
					case 1: 
						GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
						ledState =0;
						break;
					default:
						break;
				}
			ledFlag = 0;
		}
		
		TIM3->CCR1 = 0;
		TIM3->CCR2 = 0;
		TIM3->CCR3 = 0;
		TIM3->CCR4 = 0;
		
		switch(character){
			case UP:
				TIM3->CCR2 = 100;
				TIM3->CCR3 = 100;
				break;
			case DOWN:
				TIM3->CCR1 = 100;
				TIM3->CCR4 = 100;
				break;
			case LEFT:
				TIM3->CCR2 = 100; //Right forwards
				TIM3->CCR4 = 100; //Left backwards
				break;
			case RIGHT:
				TIM3->CCR1 = 100; //Right backwards
				TIM3->CCR3 = 100; //Left forwards
				break;
			case 'z': 		
				TIM3->CCR1 = 0;
				TIM3->CCR2 = 0;
				TIM3->CCR3 = 0;
				TIM3->CCR4 = 0;
				break;
			default: 
				TIM3->CCR1 = 0;
				TIM3->CCR2 = 0;
				TIM3->CCR3 = 0;
				TIM3->CCR4 = 0;
				character = 'z';
			break;
			
			// 2 and 3 is forwards
			// 1 and 2 is right, 3 and 4 is left  
			
			// 2 is right forwards
			// 3 is left forwards
			// 1 is right back
			// 4 is left back
			
			//#define UP    0
			//#define DOWN  1 
			//#define LEFT  2
			//#define RIGHT 3
		}
		/*
		// 2 and 3 is forwards
		// 1 and 2 is right, 3 and 4 is left
		TIM3->CCR1 = 0;
		TIM3->CCR2 = 100;
		TIM3->CCR3 = 100;
		TIM3->CCR4 = 0;
		*/
		//sprintf(buffer, "%u \n", ADC_values[0]);
		//USARTSend(buffer, sizeof(buffer));
		
		status = 0;
	}
}

void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		ledFlag = 1;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void DMA1_Channel1_IRQHandler(void)
{
  //Test on DMA1 Channel1 Transfer Complete interrupt
  if(DMA_GetITStatus(DMA1_IT_TC1))
  {
	  status=1;
		
   //Clear DMA1 interrupt pending bits
    DMA_ClearITPendingBit(DMA1_IT_GL1);
  }
}

void USARTSend(char *pucBuffer, unsigned long ulCount)
{
    //
    // Loop while there are more characters to send.
    //
    while(ulCount--)
    {
        USART_SendData(USART2, *pucBuffer++);// Last Version USART_SendData(USART1,(uint16_t) *pucBuffer++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
        {
        }
    }
}

void USART1_IRQHandler() {
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {//When receive
		character = (unsigned char) USART_ReceiveData(USART1); 
		state = 0;
	}	
	
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET){//When ok to transmit	
		switch(state){
			case 0: //will send
				USART_SendData(USART1, character);			
				state = 1;
				break;
			case 1: 
				break;
			default: break;
		}
	}
}