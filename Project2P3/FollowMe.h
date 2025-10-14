/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

#include "tm4c123gh6pm.h"

void stop_the_car(void){
		WHEEL_DIR = STOP;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);
}

void move_backward(void){
		WHEEL_DIR = BACKWARD;
		PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
		Timer1A_Delay(ONE_SEC);
}
	
void move_forward(void){
		WHEEL_DIR = FORWARD;
		PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
		Timer1A_Delay(ONE_SEC);
}