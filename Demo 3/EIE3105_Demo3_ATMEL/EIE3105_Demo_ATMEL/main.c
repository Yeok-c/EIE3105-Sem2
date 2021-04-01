/*
 * EIE3105_Demo_ATMEL.c
 *
 * Created: 2/17/2021 6:11:13 PM
 * Author : yeokc
 *
 * Notes:
 * PORTB |= (1 << 5);        // set only bit 5 - all others remain unchanged
 * PORTB &= ~(1 << 5);       // clear only bit 5 - all others remain unchanged
 * PORTB ^= (1 << 5);        // toggle only bit 5 - all others remain unchanged
 * PORTB ^= (1 << PB3);   // toggles the state of the bit
 
 * PB0  SW20   SW30
 * PB1  SW21   SW31
 * PB2  SW22   SW32
 * GND  SW2    SW3
 *      PD2    PD3    3.3V
 
 */ 
 
 #include <avr/io.h>
 #include <avr/interrupt.h>

 #define BAUD  9600
 #define FOSC  16000000
 #define ubrr  FOSC/BAUD/16 -1
 

#define UP    'w'
#define DOWN  's' 
#define LEFT  'a'
#define RIGHT 'd'
#define TRIANGLE 'i'
#define CIRCLE   'l'
#define CROSS    'k'
#define SQUARE   'j'

char inputFlag = 0;


void INT_Init(void){

	//set pins as outputs
	//  pinMode(2, INPUT_PULLUP);
	//  pinMode(3, INPUT_PULLUP);
	cli();//stop interrupts

	//set timer0 interrupt at 2kHz
	TCCR0A = 0;  TCCR0B = 0; TCNT0  = 0;
	OCR0A = 124;// 2kHz = (16*10^6) / (2000*64) - 1 (must be <256)
	TCCR0A |= (1 << WGM01); // turn on CTC mode
	TCCR0B |= (1 << CS01) | (1 << CS00); // 64 prescaler
	TIMSK0 |= (1 << OCIE0A); // Enable timer0 compare interrupt

	//Timer 1 Interrupt @1Hz
	TCCR1A = 0;  TCCR1B = 0; TCNT1  = 0; //Reset all variables
	OCR1AH = 0x3D;
	OCR1AL = 0x09; //Identical:  OCR1A = 15624; // = (16*10^6) / (1*1024) - 1 (must be <65536)
	TCCR1A = 0x00;
	TCCR1B = 0x0D; //1024 Prescaler, CTC Mode (0b00001101) //Identical: TCCR1B |= (1 << CS12) | (1 << CS10);  TCCR1B |= (1 << WGM12);
	TIMSK1 = (1 << OCIE1A) ;   // Enable timer1 overflow interrupt(TOIE1)

	//Use Timer 2 as 40 micro sec delay
	TCCR2A = 0; TCCR2B = 0; TCNT2  = 0;
	OCR2A = 79; // = (16*10^6) / (25000*8) - 1 (must be <256) //therefore its 40 micro sec 
	TCCR2A |= (1 << WGM21);  // turn on CTC mode
	TCCR2B |= (1 << CS21);   // Set CS21 bit for 8 prescaler
	TIMSK2 |= (1 << OCIE2A); // Enable timer2 compare interrupt
	sei();
}

 
void USART_Init(void){
	UBRR0H = (ubrr>>8);    // Set baud rate
	UBRR0L = (ubrr);  	 
	UCSR0C = 0x06;       /* Set frame format: 8data, 1stop bit  00000110*/
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<UDRIE0)|(1<<RXCIE0);
}

char USART_ReceiveByte(){ //UNUSED IN INTERRUPTS --------- Will wait until byte is received 
	char tChar = 0;
	while ( !(UCSR0A & (1<<RXC0)) ) //While theres no input
	tChar = UDR0;
	return tChar;
}

/*
void USART_SendByte(char charToPrint){ // UNUSED IN INTERRUPTS -------- Will transmit almost instantly (as soon as buffer is empty)
	while (!( UCSR0A & (1<<UDRE0))); // Wait for empty transmit buffer
	UDR0 = charToPrint;              // Put data into buffer, sends the data 
}

ISR(USART_RX_vect){		 // Receive
	currentChar = UDR0;
}
*/

unsigned char Transmitted = 1;

ISR(USART_UDRE_vect){		 // Transmit
	//Transmitted = 1;
	//UDR0 = '2';
}

int main(void){
	USART_Init();
	
	DDRB = 0xFF; //PORTB output port
	PORTB = 0x00; //Output 0 to all B ports

	//DDRC = 0xFF; //PORTC as output
	//PORTC = 0x00;

	//  DDRD = 0b00000000; //PORTD 0-5 as Input, 6-7 as Output
	DDRD = 0x00; //PORTD as input
	PORTD = 0xFF; //Write to PORTD (Activate Pullup)
	
	INT_Init();
	while (1){
		getReadings();
		writeReadings();
		clearReadings();
	}
}

char delay_small_flag = 1;
void delay_small(){
	while(delay_small_flag == 0);
}


int B_Read[8];

void writeReadings(){
	unsigned char C_Read = 0;
	if(inputFlag == 1){
		for(int i = 0; i < 8; i++){
			C_Read += B_Read[i] << i;
		}
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
		//Serial.write('\n');
	}
}

void clearReadings(){
	for(int i = 0; i < 8; i++){
		B_Read[i] = 0;
	}
	inputFlag = 0;
}

void getReadings(){
	//if(PIND&(1<<PD2) != 0){
	//Use mask to check PD2 bit and then shift it so its 0 or 1 instead of 00010000 or something
	PORTB = 0x00; //Output 0 to all B ports
	delay_small();
	
	if((PIND & (1 << PD2)) >> PD2 == 0){
		inputFlag = 1;
		//Something on left side is pressed
		char input = 0;
		PORTB = 0x03;
		delay_small();
		input |= (PIND & (1 << PD2)) >> PD2 << 1;
		//PIND & (1 << PD2) is a value like 00100000
		//>>PD2 shifts the "1" to the last byte
		//Write that to bit 1 of input
		
		PORTB = 0x05;
		delay_small();
		input |= (PIND & (1 << PD2)) >> PD2 << 2;
		
		PORTB = 0x06;
		delay_small();
		input |= (PIND & (1 << PD2)) >> PD2 << 3;
		
		if(input == 0b00000000) B_Read[0] = 1;
		if(input == 0b00001010) B_Read[1] = 1;
		if(input == 0b00001100) B_Read[2] = 1;
		if(input == 0b00000110) B_Read[3] = 1;
	
	}
	
	PORTB = 0x00; //Output 0 to all B ports
	delay_small();
	if((PIND & (1 << PD3)) >> PD3 == 0){
		inputFlag = 1;
		//Something on left side is pressed
		char input = 0;
		PORTB = 0x03;
		delay_small();
		input |= (PIND & (1 << PD3)) >> PD3 << 1;
		
		PORTB = 0x05;
		delay_small();
		input |= (PIND & (1 << PD3)) >> PD3 << 2;
		
		PORTB = 0x06;
		delay_small();
		input |= (PIND & (1 << PD3)) >> PD3 << 3;
		
		if(input == 0b00000000) B_Read[4] = 1;
		if(input == 0b00001010) B_Read[5] = 1;
		if(input == 0b00001100) B_Read[6] = 1;
		if(input == 0b00000110) B_Read[7] = 1;
	}
}

ISR(TIMER0_COMPA_vect){
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)

}

ISR(TIMER2_COMPA_vect){//40 micro sec clock
	delay_small_flag = 1;
}