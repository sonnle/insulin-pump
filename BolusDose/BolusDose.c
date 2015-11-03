/*
 * BolusDose.c
 *
 *  Created on: Aug 31, 2015
 *      Author: sle
 */

#include "Control.c"
#include "BolusDose.h"

// Global variable
uint32_t BolusDose_DoseAmountCounter;

// Counter holding number of times external interrupt 0 triggered
volatile unsigned int eint0_count;

// Setup External Interrupt 0 
void BolusDose_DoseEnable(void)
{
	// Set PINSEL4 [21:20] = 01 for P2.10 as EINT0
	LPC_PINCON->PINSEL4 &= ~(0x11 << 20);
	LPC_PINCON->PINSEL4 |= (0x01 << 20);

	// EINT0 edge trigger
	LPC_SC->EXTMODE |= EINT0_EDGE;
	
	// EINT0 is falling edge by default		
	LPC_SC->EXTPOLAR &= ~(1 <<0);	
	
	// Setup counter for number of ExtInt0's received
	eint0_count = 0;

	// Enabled External Interrupt 0 (the ISP button on RDB1768).
	NVIC_EnableIRQ(EINT0_IRQn);  

}

void BolusDose_DoseInject(void)
{
	LPC_TIM1->IR |= 1 << 0; // Clear MR0 interrupt flag
	LPC_GPIO1->FIOPIN ^= 1 << 31; // Toggle the LED
	
	// Call the stepper motor
	StepperMotor_StepForward();
}
