/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

#include "Motors.h" 

void 	move_forward(void){
	WHEEL_DIR = FORWARD;					// Enable left and right motors
	PWM0_ENABLE_R |= 0x0000000C;	// Enable PB54:PWM1 output 2&3
}

void 	move_backward(void){
	WHEEL_DIR = BACKWARD;					// Enable
	PWM0_ENABLE_R |= 0x0000000C;	// Enable PB54:PWM1 output 2&3
}

void 	stop_the_car(void){
	WHEEL_DIR = STOP;							// Disable left and right motors
	PWM0_ENABLE_R &= ~0x0000000C;	// Enable PB54:PWM1 output 2&3
}