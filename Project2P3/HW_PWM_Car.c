/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "PWM.h"
#include "GPIO.h"
#include <stdint.h>
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////
#define SW1_MASK	0x10
#define ONE_SEC		64000		// 16000000/(prescale+1) = 16000000/250 

volatile uint32_t SW1_PRESS = 0;

////////// Function Prototypes //////////
void DisableInterrupts(void);
void EnableInterrupts(void);
void WaitForInterrupt(void);
void PortF_Init(void);
void Timer1A_Init(void);
void Timer1A_Delay(unsigned long period);

//////////////////////3. Subroutines Section/////////////////////////////////
// MAIN: This main is meant for the command configuration of the hc05.
int main(void){ 
	Timer1A_Init();
  PortF_Init();
  Car_Dir_Init();
  PWM_PB54_Init();
  PWM_PB54_Duty(START_SPEED, START_SPEED);
  
	while(1){
		SW1_PRESS = 0;
		while(!SW1_PRESS);
		
		// (1) move forward, 50% PWM duty cycle
		WHEEL_DIR = FORWARD;
		PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
		Timer1A_Delay(ONE_SEC);
		
		// stop 
		WHEEL_DIR = STOP;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);

		// (2) move backward, 50% PWM duty cycle
		WHEEL_DIR = BACKWARD;
		PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
		Timer1A_Delay(ONE_SEC);
		
		// stop 
		WHEEL_DIR = STOP;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);

		// (3) forward left, 20% PWM duty cycle
		PWM_PB54_Duty(TWENTY_PERC, TWENTY_PERC);
		WHEEL_DIR=FORWARD;
		PWM0_ENABLE_R |= 0x00000008; // Enable right wheel
		PWM0_ENABLE_R &= ~0x00000004; // Disable left wheel
		Timer1A_Delay(ONE_SEC);
		
		// stop 
		WHEEL_DIR = STOP;		
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);

		// (4) forward right, 20% PWM duty cycle
		WHEEL_DIR=FORWARD;
		PWM0_ENABLE_R &= ~0x00000008; // Disable right wheel
		PWM0_ENABLE_R |= 0x00000004; // Enable left wheel
		Timer1A_Delay(ONE_SEC);
		
		// stop 
		WHEEL_DIR = STOP;		
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);

		// (5) backward left, 20% PWM duty cycle
		WHEEL_DIR = BACKWARD;
		PWM0_ENABLE_R &= ~0x00000008; // Disable right wheel
		PWM0_ENABLE_R |= 0x00000004; 	// Enable left wheel
		Timer1A_Delay(ONE_SEC);
		
		// stop 
		WHEEL_DIR = STOP;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);

		// (6) backward right, 20% PWM duty cycle
		WHEEL_DIR=BACKWARD;
		PWM0_ENABLE_R |= 0x00000008; // Enable right wheel
		PWM0_ENABLE_R &= ~0x00000004; // Disable left wheel
		Timer1A_Delay(ONE_SEC);
		
		// stop 
		WHEEL_DIR = STOP;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);

		// (7) pivot left, 50% PWM duty cycle
		PWM_PB54_Duty(START_SPEED, START_SPEED);
		WHEEL_DIR=LEFTPIVOT;
		PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
		Timer1A_Delay(ONE_SEC);

		// stop 
		WHEEL_DIR = STOP;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);

		// (8) pivot right, 50% PWM duty cycle
		WHEEL_DIR=RIGHTPIVOT;
		PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
		Timer1A_Delay(ONE_SEC);

		// stop 
		WHEEL_DIR = STOP;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);
	
		// (9) stop 
		WHEEL_DIR = STOP;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(ONE_SEC);
	}
}

void GPIOPortF_Handler(void){
	// simple debouncing code
	for (uint32_t time=0;time<160000;time++){}
	
	if (GPIO_PORTF_RIS_R & SW1_MASK){
		GPIO_PORTF_ICR_R |= SW1_MASK;
		SW1_PRESS = 1;	// acknowledge flag4: 00010000 for SW1
	}
}