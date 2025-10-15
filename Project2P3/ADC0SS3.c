// ADCPD0SS3.c
// Runs on TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// Daniel Valvano
// October 20, 2013
// Modified by Min He, 10/09/2022

// This file provide initialization function for two analog channels:
// PE2/AIN1 and PD0/AIN7




// this file is going to be a ADC1SS2 module to take care of three sensor outputs
// example project look at  WallFollower
// add two more sensors to robot and implement function object_follower in following_robot.c
// default state is to be in mode 1
#include "tm4c123gh6pm.h"
#include <stdint.h>

#define ADC0_PSSI_SS3 0x0008    // start sample sequencer 3
#define ADC0_ISC_SS3  0x0008    // acknowledge sample sequencer 3 interrupt
#define ADC0_RIS_SS3  0x08

// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain7 (PD0)
// SS3 interrupts: flag set on completion but no interrupt requested
// When the PLL is operating, the ADC clock is derived from the PLL(400) � 25 by default.

// Initializes AIN2, AIN9, and AIN8 sampling
// 125k max sampling
// SS2 triggering event: software trigger, busy-wait sampling
// SS2 1st sample source: AIN2 (PE1)
// SS2 2nd sample source: AIN9 (PE4)
// SS2 3rd sample source: AIN8 (PE5)
// SS2 interrupts: enabled after 3rd sample but not promoted to controller
void ADC_Init298(void){

  
  volatile unsigned long delay;
//  SYSCTL_RCGC0_R |= 0x00010000; // 1) activate ADC0 (legacy code)
  SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0x32;      // 3) make PE1, PE4, and PE5 input
  GPIO_PORTE_AFSEL_R |= 0x32;     // 4) enable alternate function on PE1, PE4, and PE5
  GPIO_PORTE_DEN_R &= ~0x32;      // 5) disable digital I/O on PE1, PE4, and PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFF00FF0F;
  GPIO_PORTE_AMSEL_R |= 0x32;     // 6) enable analog functionality on PE1, PE4, and PE5
  ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC0_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC0_SSMUX2_R = 0x0892;         // 12) set channels for SS2
  ADC0_SSCTL2_R = 0x0600;         // 13) no D0 END0 IE0 TS0 D1 END1 IE1 TS1 D2 TS2, yes END2 IE2
  ADC0_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
}

//------------ADC_In298------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: three 12-bit result of ADC conversions
// Samples AIN8, AIN9, and AIN2
// 125k max sampling
// software trigger, busy-wait sampling
// data returned by reference
// ain2 (PE1) 0 to 4095
// ain9 (PE4) 0 to 4095
// ain8 (PE5) 0 to 4095
void ADC_In298(unsigned long *ain2, unsigned long *ain9, unsigned long *ain8){
  ADC0_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
  *ain2 = ADC0_SSFIFO2_R&0xFFF;    // 3A) read first result
  *ain9 = ADC0_SSFIFO2_R&0xFFF;    // 3B) read second result
  *ain8 = ADC0_SSFIFO2_R&0xFFF;    // 3C) read third result
  ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
}



/*
void ADC0_InitSWTriggerSeq3_Ch7(void){ 
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;   // 1) activate clock for Port D
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R3)!=SYSCTL_RCGCGPIO_R3){}

	GPIO_PORTD_DIR_R &= ~0x01;      // 2) make PD0 input
  GPIO_PORTD_AFSEL_R |= 0x01;     // 3) enable alternate function on PD0
  GPIO_PORTD_DEN_R &= ~0x01;      // 4) disable digital I/O on PD0
  GPIO_PORTD_AMSEL_R |= 0x01;     // 5) enable analog function on PD0
  
	SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
	while ((SYSCTL_RCGC0_R&0x00010000)!=0x00010000){}
         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+7; // 11) channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3
}

// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain1 (PE2)
// SS3 interrupts: flag set on completion but no interrupt requested
void ADC0_InitSWTriggerSeq3_Ch1(void){ 
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;   // 1) activate clock for Port E
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R4)!=SYSCTL_RCGCGPIO_R4){}

	GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;   // 6) activate ADC0 
	while ((SYSCTL_RCGC0_R&0x00010000)!=0x00010000){}
         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K 
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008;         // 13) enable sample sequencer 3
}

//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC0_InSeq3(void){  
	uint16_t result;
  ADC0_PSSI_R = ADC0_PSSI_SS3;            // 1) initiate SS3:1000
  while((ADC0_RIS_R&ADC0_RIS_SS3)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = ADC0_ISC_SS3;             // 4) acknowledge completion
  return result;
}
*/