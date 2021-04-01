#include "stdio.h"
#include "misc.h"
#include "string.h"

#include "stm32f10x.h"                  // Device header
#include "pinout.h"
//#include "USART1_F103C8.h"
//#include "INDICATOR_LED_TIM2.h"
//#include "PWM_4CH_TIM3.h"
#include "EXT_INT.h"
#include "SPI.h"
#include "PID.h"




/*
	Cheah Yeok Tatt
	18078696D 
	Demo 3 Code
*/


//Function Prototypes (Only useful functions)
void INDICATOR_LED_init(void);	
void TIM3_PWM_init(void);
void TIM2_init(void);
void USART1_init(void);

void char_to_bin(unsigned char c);
int count_black(int *arr);
int average_black(int *arr, int black_counts);
void USART_Send_String(char *pucBuffer, unsigned long ulCount);
void PID_DRIVE(int ref_speed_L, int ref_speed_R, int count_L, int count_R, int dir_L, int dir_R);
void switchStage(void);
void init_PID(void);

unsigned char ledState=0;
unsigned long tim2_count=0;

//GLOBAL VARIABLES

int ref_speed_L = 20;
int ref_speed_R = 20;
int dir_L = 1;
int dir_R = 1;

int main(void) {
	char buffer[50] = {'\0'};
	
	INDICATOR_LED_init();
	TIM3_PWM_init();
	TIM2_init();
	USART1_init();
	
	Left_Wheel_Cnt_init();
	Right_Wheel_Cnt_init();
	init_PID();
	//Button_init();
	spi2_init();
	
	while(1) {
		
	//Direct Circumference Measurement = 17.5cm
	//Radius = 5.5cm -> Calculated Circumference = 17.27cm -> We take 17.3
	//From experiments, we know each rotation = 60 encoder counts
	//Hence each encoder count = 17.3cm/60 = 0.288333cm/encoder -> 2.8833cm per 10 encoder 

	if(TIM2_Flag == 1){ //Enter every 25ms
		char_to_bin(c);
		int black_count = count_black(bits);
		float black_avg = average_black(bits, black_count);
		int quantized_black_avg = black_avg*2;
		readFloor();
		int no_pid_flag = 0;
		
		switchStage();		
		switch(ledState){
				case 0: //Timer is done and waiting for next call to ledState = 1
					GPIO_WriteBit(LED_INT_GPIO, LED_INT_PIN, Bit_RESET);
					GPIO_WriteBit(LED_Y_GPIO, LED_Y_PIN, Bit_RESET);
					GPIO_WriteBit(LED_G_GPIO, LED_G_PIN, Bit_RESET);
					ledState =1;
					break;
				case 1: //Timer is active						
									
					//sprintf(buffer, "Current Readings: %d ; in bits: %d %d %d %d %d %d %d %d \n", c, bits[0], bits[1], bits[2], bits[3], bits[4], bits[5], bits[6], bits[7]);
					//sprintf(buffer, "Left_Count: %02d, Right_Count: %02d \n ", CNT_L, CNT_R);
					//USART_Send_String(buffer, sizeof(buffer));
					if(character_received == 1){
						switch(character){
							//For remote control (Details in pinout.h)
							case Up:
								ref_speed_L = 20;
								ref_speed_R = 20;
								dir_L = 1;
								dir_R = 1;
								tim2_count = 1;
								break;
							case Down:					
								ref_speed_L = 20;
								ref_speed_R = 20;
								dir_L = 0;
								dir_R = 0;
								tim2_count = 1;
								break;
							case Left://Stationary Left Rotation
								ref_speed_L = 5;
								ref_speed_R = 5;
								dir_L = 0;
								dir_R = 1;
								tim2_count = 1;
								break;
							case Right://Stationary Right Rotation
								ref_speed_L = 5;
								ref_speed_R = 5;
								dir_L = 1;
								dir_R = 0;
								tim2_count = 1;
								break;
							
							case Up_Shift_Left: //Forwards while moving Left 
								ref_speed_L = 6;
								ref_speed_R = 20;
								dir_L = 1;
								dir_R = 1;
								tim2_count = 1;
								break;
							case Down_Shift_Left: //Backwards while moving Left 
								ref_speed_L = 6;
								ref_speed_R = 20;
								dir_L = 0;
								dir_R = 0;
								tim2_count = 1;
								break;
							case Up_Shift_Right: //Forwards while moving Right 
								ref_speed_L = 20;
								ref_speed_R = 6;
								dir_L = 1;
								dir_R = 1;
								tim2_count = 1;
								break;
							case Down_Shift_Right: //Backwards while moving Right 
								ref_speed_L = 20;
								ref_speed_R = 6;
								dir_L = 0;
								dir_R = 0;
								tim2_count = 1;
								break;
							
							//For tuning with Teraterm
							case '7': 
								Ku = Ku + 5;
								break;
							case '4':
								Ku = Ku - 5;
								break;
							case '8':
								Kd = Kd + 0.5;
								break;
							case '5':
								Kd = Kd - 0.5;
								break;
							case '9':
								Kd = Kd + 0.5;
								break;
							case '6':
								Kd = Kd - 0.5;
								break;
							default:
								break;
						}
						character_received = 0;
					}	
					if(tim2_count > 0){
						tim2_count--;						
						if(no_pid_flag == 0) PID_DRIVE(ref_speed_L, ref_speed_R, CNT_L, CNT_R, dir_L, dir_R);
						CNT_L = 0;
						CNT_R = 0;
						GPIO_WriteBit(LED_INT_GPIO, LED_INT_PIN, Bit_SET);
						GPIO_WriteBit(LED_Y_GPIO, LED_Y_PIN, Bit_SET);
						GPIO_WriteBit(LED_G_GPIO, LED_G_PIN, Bit_SET);
					} 
					else drive(1, 1, 1, 1);
					ledState =0;
					break;
					
				default:
					break;
				}
			TIM2_Flag = 0;
		}
	}
}

int STAGES[2][3] = {
	{1, 2, 3},
	{1, 2, 3}
};

void switchStage(void){
	//Stages are based on a map
	
}