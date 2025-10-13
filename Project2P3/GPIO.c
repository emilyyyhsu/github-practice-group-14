/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Project 2 Part 1
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "GPIO.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////
////////// Constants //////////

////////// Local Global Variables //////////
/////////////////////////////////////////////////////////////////////////////

//////////////////////3. Subroutines Section/////////////////////////////////
// Description: Initializes PB765432 for use with L298N motor driver direction
// PB4: Hardware PWM output for left motor
// PB5: Hardware PWM output for right motor
// PB32: Direction pins for left motor
// PB76: Direction pins for right motor
void Car_Dir_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
		
  GPIO_PORTB_AMSEL_R &= ~0xCC;	// disable analog function 
	GPIO_PORTB_AFSEL_R &= ~0xCC;	// no alternate function
  GPIO_PORTB_PCTL_R &= ~0xFF00FF00;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= 0xCC; // output on pin(s)
  GPIO_PORTB_DEN_R |= 0xCC;	// enable digital I/O on pin(s)
}

// Port F Initialization
// Enable interrupt for SW1 on PF4
void PortF_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
		
	GPIO_PORTF_CR_R |= 0x10; // allow changes to PF4 :10000->0x10
	GPIO_PORTF_AMSEL_R &= ~0x10; // disable analog function
	GPIO_PORTF_PCTL_R &= ~0x000F0000; // GPIO clear bit PCTL
	GPIO_PORTF_PUR_R |= 0x10; // enable pull upresistor on pf4
	GPIO_PORTF_DIR_R &= ~0x10; // PF4 input
	GPIO_PORTF_AFSEL_R &= ~0x10; // no alternate function
	GPIO_PORTF_DEN_R |= 0x10; // enable digital pins PF4
	GPIO_PORTF_IS_R &= ~0x10;
	GPIO_PORTF_IBE_R &= ~0x10; 	// PF4 is edge-sensitive
	GPIO_PORTF_IEV_R |= 0x10;	// PF4 is not both edges
	GPIO_PORTF_ICR_R |= 0x10;	// PF4 rising edge event
	GPIO_PORTF_IM_R |= 0x10;	// Clear Flag4
														// Arm interrupt on PF4
	NVIC_EN0_R = 0x40000000; // (h) enable Port B edge interrupt
}

/////////////////////////////////////////////////////////////////////////////
