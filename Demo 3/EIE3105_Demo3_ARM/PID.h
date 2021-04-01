#include "PWM_4CH_TIM3.h"
#include "USART1_F103C8.h"
#include "INDICATOR_LED_TIM2.h"

//CONSTANTS THAT CAN BE CHANGED
float Ku = 60;
float Tu = 2.5;
float Kp = 20; //90
float Ki = 1; //2250.000000
float Kd = 3; //9.000000

//According to Ziegler-Nichols Method (which i did not end up using LOL)
//Step 1: Set Kp = small, Ki = Kd = 0;

//Step 2: Increase Kp until it oscilates forever (marginally stable)

//Step 3: Record critical/ultimate gain Ku = Kp @ Marginal Stability 
//				-> by experimentation, Ku = 25

//Step 4: Look Up KP, Ti, Td
//				->For classic PID, Kp = 0.6Ku, Ti = Tu/2, Td = Tu/8

//Step 5: Compute Ki, Kd 
//				->Ki = 12*Ku/Tu
//				->Kd = 0.075*Ku*Tu

#define BASEPWM 100
#define MAXPWM 220
int Ts = TIM2_PERIOD;
int RANGE = MAXPWM - BASEPWM;
int base_pwm_L = BASEPWM;
int base_pwm_R = BASEPWM;

//PRIVATE GLOBAL VARIABLES
float lastError_L;
float lastError_R;

//Function Prototypes
int PID_LEFT(int errorL);
int PID_RIGHT(int errorR);
void drive(int posL, int dirL, int posR, int dirR);

void init_PID(void){
/*
	//if using ziegler nichols method
	Kp = 0.6*Ku;
	float Ti = Tu/2;
	float Td = Tu/8;
	Ki = Kp/Ti;
	Kd = Kp*Td;
*/
}
//Main function
void PID_DRIVE(int ref_speed_L, int ref_speed_R, int count_L, int count_R, int dirL, int dirR){
	int error_L = ref_speed_L - count_L;		//Positive error = Reference Speed Faster -> Need increase in Power
	int error_R = ref_speed_R - count_R;		//Positive error = Reference Speed Faster -> Need increase in Power
	int leftPWM = PID_LEFT(error_L);
	int rightPWM = PID_RIGHT(error_R);
	drive(leftPWM, dirL, rightPWM, dirR);
	
	//char buffer[120] = {'\0'};
	//sprintf(buffer, "Kp: %01f, Ki: %01f, Kd: %01f, C_L: %03d, C_R: %03d , E_L: %03d, E_R: %03d, PWM_L: %03d, PWM_R: %03d \n", Kp, Ki, Kd, count_L, count_R, error_L, error_R, leftPWM, rightPWM);	
	//USART_Send_String(buffer, sizeof(buffer));
}

/*
INPUTS:  TARGET WAYPOINT POSITION
				 CURRENT POSITION
OUTPUTS: PWM SPEED
*/

int PID_LEFT(int error_L){
	float P = error_L; 								//error
	float I = I + error_L;						//error+all previous error
	float D = (error_L - lastError_L)/Ts;	//delta error/t where t = sampling period
	lastError_L = error_L;

	float temp_speedInc_L = P*Kp + I*Ki + D*Kd;
	int speedInc_L = (int) temp_speedInc_L;
	int motor_pwm_L = base_pwm_L + speedInc_L;

	if (motor_pwm_L > MAXPWM) {
		motor_pwm_L = MAXPWM;
	}
	if (motor_pwm_L < 0) {
		motor_pwm_L = 0;
	}
	return motor_pwm_L;
}

int PID_RIGHT(error_R){
	float P = error_R;
	float I = I + error_R;
	float D = error_R - lastError_R;
	lastError_R = error_R;

	float temp_speedInc_R = P*Kp + I*Ki + D*Kd;
	int speedInc_R = (int) temp_speedInc_R;
	int motor_pwm_R = base_pwm_R + speedInc_R;

	if (motor_pwm_R > MAXPWM) {
		motor_pwm_R = MAXPWM;
	}
	if (motor_pwm_R < 0) {
		motor_pwm_R = 0;
	}
	return motor_pwm_R;
}

//	drive(motorspeedL, motorspeedR); //Drive
void drive(int posL, int dirL, int posR, int dirR) {
	TIM3->CCR1 = 1;
	TIM3->CCR2 = 1;
	TIM3->CCR3 = 1;
	TIM3->CCR4 = 1;
	if(dirL == 1) TIM3->CCR2 = posL; // LEFT WHEEL 
	else if (dirL == 0) TIM3->CCR1 = posL;
	if(dirR == 1) TIM3->CCR3 = posR; // RIGHT WHEEL
	else if(dirR == 0)	TIM3->CCR4 = posR;
}



