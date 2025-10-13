#include "tm4c123gh6pm.h"
#include "PWM.h"
#include <stdint.h>

#define ONE_SEC 64000

void Timer1A_Delay(unsigned long period);

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