//Usage
// PA2 - USART2 TX
// PA3 - USART2 RX

// PA6 - Tim3 Ch1 PWM
// PA7 - Tim3 Ch2 PWM
// PB0 - Tim3 Ch3 PWM
// PB1 - Tim3 Ch4 PWM

// PA0 - BUTT_0
// PC13- BUTT_1

// PB12 - LED_Internal
// PA5 - LED_Y
// PA8 - LED_G

// PA1 - CNT_L 
// PB7 - CNT_R

/*
#define UP    'w'
#define DOWN  's' 
#define LEFT  'a'
#define RIGHT 'd'
#define TRIANGLE 'i'
#define CIRCLE   'l'
#define CROSS    'k'
#define SQUARE   'j'
*/
#define Up    'w' 
#define Left  'a'
#define Right 'd'
#define Down  'x'
#define Up_Shift_Left    'q' 
#define Down_Shift_Left  'z'
#define Up_Shift_Right 'e'
#define Down_Shift_Right  'c'
/*
		switch(C_Read){
			case 1: UDR0 = 'w'; break;	 //Up
			case 4: UDR0 = 'a'; break;	 //Left
			case 8: UDR0 = 'd'; break;	 //Right
			case 2: UDR0 = 'x'; break;	 //Down
			case 129: UDR0 = 'q'; break; //Up_Shift_Left
			case 130: UDR0 = 'z'; break; //Down_Shift_Left
			case 33: UDR0 = 'e'; break;  //Up_Shift_Right
			case 34: UDR0 = 'c'; break;  //Down_Shift_Right
		}
		*/

// PB12 - LED_Internal
#define LED_INT_RCC_GPIO  RCC_APB2Periph_GPIOB
#define LED_INT_GPIO      GPIOB
#define LED_INT_PIN       GPIO_Pin_12

// PA5 - LED_Y
#define LED_Y_RCC_GPIO  RCC_APB2Periph_GPIOA
#define LED_Y_GPIO      GPIOA
#define LED_Y_PIN       GPIO_Pin_5

// PA8 - LED_G
#define LED_G_RCC_GPIO  RCC_APB2Periph_GPIOA
#define LED_G_GPIO      GPIOA
#define LED_G_PIN       GPIO_Pin_8

// PA0 - BUTT_0 External Interrupt
#define B0_RCC_GPIO        RCC_APB2Periph_GPIOA
#define B0_GPIO	           GPIOA
#define B0_GPIO_PIN        GPIO_Pin_0
#define B0_EXTI_LINE       EXTI_Line0
#define B0_GPIO_PORTSOURCE GPIO_PortSourceGPIOA
#define B0_GPIO_PINSOURCE  GPIO_PinSource0


// PC13- BUTT_1 External Interrupt
#define B1_RCC_GPIO        RCC_APB2Periph_GPIOC
#define B1_GPIO	           GPIOC
#define B1_GPIO_PIN        GPIO_Pin_13
#define B1_EXTI_LINE       EXTI_Line13
#define B1_GPIO_PORTSOURCE GPIO_PortSourceGPIOC
#define B1_GPIO_PINSOURCE  GPIO_PinSource13

/*
// PA1 - CNT_L 
#define B0_RCC_GPIO        RCC_APB2Periph_GPIOA
#define B0_GPIO	           GPIOA
#define B0_GPIO_PIN        GPIO_Pin_0
#define B0_EXTI_LINE       EXTI_Line0
#define B0_GPIO_PORTSOURCE GPIO_PortSourceGPIOA
#define B0_GPIO_PINSOURCE  GPIO_PinSource0

// PB7 - CNT_R
#define B1_RCC_GPIO        RCC_APB2Periph_GPIOC
#define B1_GPIO	           GPIOC
#define B1_GPIO_PIN        GPIO_Pin_13
#define B1_EXTI_LINE       EXTI_Line13
#define B1_GPIO_PORTSOURCE GPIO_PortSourceGPIOC
#define B1_GPIO_PINSOURCE  GPIO_PinSource13
*/

//Four PWM enabled Channels
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
