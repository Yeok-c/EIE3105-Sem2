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

// 4 ADC Channels
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


//B1 PC13 External Interrupt
#define B1_RCC_GPIO        RCC_APB2Periph_GPIOC
#define B1_GPIO	           GPIOC
#define B1_GPIO_PIN        GPIO_Pin_13
#define B1_EXTI_LINE       EXTI_Line13
#define B1_GPIO_PORTSOURCE GPIO_PortSourceGPIOC
#define B1_GPIO_PINSOURCE  GPIO_PinSource13

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
