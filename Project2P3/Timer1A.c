// Timer1A.c
// Runs on LM4F120/TM4C123
// Use TIMER1A in 16-bit periodic countdown mode to request interrupts at a periodic rate
// Ana Arante, Hanna Estrada, Emily Hsu, Suphia Sidiqi
// September 8, 2025

#include "tm4c123gh6pm.h"
#include "PLL.h"
#include <stdint.h>

// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

void Timer1A_Init(void){
	SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
	while((SYSCTL_RCGCTIMER_R&0x02)!=0x02){}; // wait for clock to start
	TIMER1_CTL_R = 0x00000000; 		// disable TIMER1A
	TIMER1_CFG_R = 0x00000004;    // 2) configure for 16-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic down-count mode
	TIMER1_TAPR_R = 249;         	// 5) bus clock prescale
}

void Timer1A_Delay(unsigned long period){
	TIMER1_CTL_R = 0x00000000; 		// disable TIMER1A
	TIMER1_TAILR_R = period-1;
	TIMER1_CTL_R = 0x00000001;
	while((TIMER1_TAR_R) != 0){}
}

