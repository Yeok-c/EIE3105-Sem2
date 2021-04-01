#include "stm32f10x.h"                  // Device header
//#include "ADC_DMA.h"
#include "pinout.h"
#include "USART1_F103C8.h"
#include "INDICATOR_LED_TIM2.h"
#include "PWM_4CH_TIM3.h"
#include "SPI.h"

#include "stdio.h"
#include "misc.h"
#include "string.h"


#define UP    '0'
#define DOWN  '1' 
#define LEFT  '2'
#define RIGHT '3'
#define TRIANGLE '4'
#define CIRCLE   '5'
#define CROSS    '6'
#define SQUARE   '7'


/*
	Cheah Yeok Tatt
	18078696D 
	AVR and ARM Test 2021 Sem 2
	
	Q2:
	When string ABC_ is sent - repeated in tera term
	When covered by hand - XYZ_ is shown every second
*/

// Internal Macros
#define HEX__(n) 0x##n##LU
#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)

// User-visible Macros
#define B8(d) ((unsigned char)B8__(HEX__(d)))
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) + B8(dlsb))
#define B32(dmsb,db2,db3,dlsb) \
(((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))

/*
Here are some examples of the usage of these macros:
B8(01010101) // 85
B16(10101010,01010101) // 43,605
B32(10000000,11111111,10101010,01010101) // 2,164,238,933
*/


void INDICATOR_LED_init(void);	
void TIM3_PWM_init(void);
void TIM2_init(void);
void switchStage();
void USARTSend(char *pucBuffer, unsigned long ulCount);
int count_black(int *arr);
void drive(int bias);

char xyz[5] = {"XYZ_\t"};
char abc[5] = {"ABC_\t"};
unsigned char ledState=0;
unsigned char character, newCharacter, state;
unsigned char driveFlag = 0;
int bits[8];

#define HIGH 90
int MAXD = 10;
int MIDD = 8;
int MIND = 2;
int K = HIGH/10;
int K2 = 2;
int black_count = 0;
int black_avg = 0;
unsigned int count = 0;
int main(void) {
	
	char buffer[50] = {'\0'};
	
	INDICATOR_LED_init();
	//ADC1_3channels_init();
	//DMA1_init();
	TIM3_PWM_init();
	TIM2_init();
	USART1_init();
	spi2_init();
	//Enable DMA1 Channel transfer
	// DMA_Cmd(DMA1_Channel1, ENABLE); // start conversion
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// start conversion (will be endless as we are in continuous mode)
	
	while(1) {
		if(ledFlag == 1){				
			black_count = count_black(bits);
			unsigned char c1 = c;
			unsigned char mask = 1; // 00000001
			for(int n = 7;n >= 0;n--) {
				bits[n] = c1 & mask; 
				// 1 if the last bit of x is 1, 0 if it is not
				c1 = c1 >> 1; // Use a shift to drop the last bit of x
			}
			readFloor();
			switchStage();
			switch(ledState){
					case 0: //Timer is done and waiting for next call to ledState = 1
						GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
						break;
					case 1: //Timer is active
						GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
						count++;
						if(count >= 400){
							ledState =0;
							count = 0;
						}
						
						break;
					default:
						break;
				}
			ledFlag = 0;
		}
		
		if(driveFlag == 1){
			driveFlag = 0;
			switch(character){
				case UP:
					TIM3->CCR1 = 1;
					TIM3->CCR2 = 1;
					TIM3->CCR3 = 1;
					TIM3->CCR4 = 1;
					TIM3->CCR2 = HIGH;
					TIM3->CCR3 = HIGH;
					//character = 'z';
					break;
				case DOWN:
					TIM3->CCR1 = 1;
					TIM3->CCR2 = 1;
					TIM3->CCR3 = 1;
					TIM3->CCR4 = 1;
					TIM3->CCR1 = HIGH;
					TIM3->CCR4 = HIGH;
					//character = 'z';
					break;
				case LEFT:
					TIM3->CCR1 = 1;
					TIM3->CCR2 = 1;
					TIM3->CCR3 = 1;
					TIM3->CCR4 = 1;
					TIM3->CCR1 = HIGH; //Right backwards
					TIM3->CCR3 = HIGH; //Left forwards
					//character = 'z';
					break;
				case RIGHT:
					TIM3->CCR1 = 1;
					TIM3->CCR2 = 1;
					TIM3->CCR3 = 1;
					TIM3->CCR4 = 1;
					TIM3->CCR2 = HIGH; //Right forwards
					TIM3->CCR4 = HIGH; //Left backwards
					//character = 'z';
					break;
				case 'z': 		
				/*	
					TIM3->CCR1 = 1;
					TIM3->CCR2 = 1;
					TIM3->CCR3 = 1;
					TIM3->CCR4 = 1;
				*/				
				break;
				default: 
					character = 'z';
				break;
			}		
		}
	}
}

int subStage = 0;
int stage = 0;
char lastc;
int DRIVE = 0;

void switchStage(void){
	//Stages are
	//	1 : A B | C D | A
	//	12: Drastic 90 degree LEFT turn at A
	//	2 : A X C
	//	23: 180 Turn at C
	//	3 : C X A
	//	34: Drastic 90 degree RIGHT turn at A
	//	4 : D C B A 
	
	
	switch(stage){
		case 0: // Put car right on line to start
				stage = 1;						
			break;
		case 1: // A B| C D| so at 4th substage, then move on to drastic turn	
		drive(0);
		if(black_count < 7){ //Not off the ground 
			if(ledState == 0){ //Only enter when timer is done counting down
				if(black_count >= 5){// bar
					subStage++;
					ledState = 1; 	 //timer start
					if(subStage >= 4){
						subStage = 0;
						stage = 12;
						ledState = 1;
					}
				}
			}
		}
			break;
		case 12: //Drastic 90 degree LEFT turn at A
		if(ledState == 1){
			TIM3->CCR2 = 0;
			TIM3->CCR3 = 110;
		}
		else {
			if(black_count > 3 ){ //re entering line
				stage = 2;
				ledState = 1; 	 //timer start
			}
		}
			break;
		case 2: //A X C
		drive(0);
		if(black_count < 7){ //Not off the ground 
			if(ledState == 0){ //Only enter when timer is done counting down
				if(black_count >= 5){// bar
					ledState = 1; 	 //timer start
					stage = 23;
					TIM3->CCR3 = 230;
					TIM3->CCR1 = 230;
				}
			}
		}
			break;
		case 23: //180 Turn at C
		if(ledState == 0){
			subStage++;
			TIM3->CCR3 = 70;
			TIM3->CCR2 = 140;
			ledState = 1;
		}
		if (subStage > 1) {
			if(black_count > 3 ){ //re entering line
				ledState = 1; 	 //timer start
				stage = 3;
				subStage = 0;
			}
		}
			break;
				
		case 3:  //C X A
		drive(0);
		if(black_count < 7){ //Not off the ground 
			if(ledState == 0){ //Only enter when timer is done counting down
				if(black_count >= 5){// bar
					ledState = 1; 	 //timer start
					stage = 34;
				}
			}
		}
			break;
		case 34: //Drastic 90 degree RIGHT turn at A
			ledState = 1;
			TIM3->CCR4 = 0;
			TIM3->CCR1 = 70;	
			stage = 4;		
			break;
			
		case 4: 
			if(ledState == 0){ //Only enter when timer is done counting down
				drive(0);
			}
			break;
	}
}

void USART1_IRQHandler() {
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {//When receive
		character = (unsigned char) USART_ReceiveData(USART1); 
		driveFlag = 1;
		state = 0;
	}	
	/*
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET){//When ok to transmit	
		switch(state){
			case 0: //will send
				//USART_SendData(USART1, character);			
				state = 1;
				break;
			case 1: 
				break;
			default: break;
		}
	}*/
}

int count_black(int *arr){
	int count = 0;
	for(int i; i < 8; i++){
		if(*(arr+i) == 1) count++;
	}
	return count;
}

float average_black(int *arr, int count){
	float average = 0;
	for(int i; i< 8; i++){
		if(*(arr+i) == 1){
			average = average + i;	
		}
	}
	if(count == 0){
		average = 0;
	} else average = average/count;
	
	return average;
}

void char_to_bin(unsigned char c){
	unsigned char c1 = c;
	unsigned char mask = 1; // 00000001
	for(int n = 7;n >= 0;n--) {
		bits[n] = c1 & mask; 
		// 1 if the last bit of x is 1, 0 if it is not
		c1 = c1 >> 1; // Use a shift to drop the last bit of x
	}
}

void drive(int bias){ //Pos bias = right wheel = turn left
	TIM3->CCR1 = 1;
	TIM3->CCR2 = 1;
	TIM3->CCR3 = 1;
	TIM3->CCR4 = 1;
	
	//Set D= -1 or +1
	int D = 1;
	if(bits[0] == 1) D = MAXD;
	if(bits[1] == 1) D = MIDD;
	if(bits[2] == 1) D = MIDD;
	if(bits[3] == 1) D = MIDD;
	if(bits[4] == 1) D = MIND;
	
	if(bits[7] == 1) D = -MAXD;
	
	if(bits[5] == 1){
		if(bits[6] == 1){ //both bit 3 and bit 4 (dead center)
			D = 0;
		}
		else D = -1; //only bit 3 (slight left)
	}
	
	/*
	if(bits[7] == 1) D = -MAXD;
	if(bits[6] == 1) D = -MIDD;
	if(bits[5] == 1)D = -MIDD;
	if(bits[4] == 1) D = -MIDD;
	if(bits[3] == 1)D = -MIDD;

	if(bits[0] == 1) D = MAXD;
	if(bits[2] == 1){
		if(bits[1] == 1){//both bit 1 and bit 2 (dead center)
			D = 0;
		}
		else D = -1; //only bit 2 (slight right)
	}
	*/

	/*
	
	if(bits[0] == 1) D = MAXD;
	if(bits[1] == 1) D = MIDD;
	if(bits[2] == 1) D = MIND;
	if(bits[3] == 1){
		if(bits[4] == 1){ //both bit 3 and bit 4 (dead center)
			D = 1;
		}
		else D = 1; //only bit 3 (slight left)
	}
	if(bits[7] == 1) D = -MAXD;
	if(bits[6] == 1) D = -MIDD;
	if(bits[5] == 1)D = -MIND;
	if(bits[4] == 1){
		if(bits[3] == 1){//both bit 3 and bit 4 (dead center)
			D = 0;
		}
		else D = -1; //only bit 3 (slight right)
	}
	*/
	//LEFT READING = TURN LEFT = POS D 
	if(black_count < 8){
		if(black_count > 0){
			TIM3->CCR2 = HIGH-K*D; // LEFT WHEEL 
			TIM3->CCR3 = HIGH+K*D; // RIGHT WHEEL
		}
		/*
		else if(BLACK_COUNT > 3){
			//sharp turn left
			TIM3->CCR2 = HIGH;
			TIM3->CCR3 = HIGH+HIGH/2;
		}	
		*/							
	}
		
	//sprintf(buffer, "Current Readings: %d ; in bits: %d %d %d %d %d %d %d %d \n", c, bits[0], bits[1], bits[2], bits[3], bits[4], bits[5], bits[6], bits[7]);
	//sprintf(buffer, "Direction: %d ; in bits: %d %d %d %d %d %d %d %d \n", D, bits[0], bits[1], bits[2], bits[3], bits[4], bits[5], bits[6], bits[7]);
	//USART_Send_String(buffer, sizeof(buffer));
}