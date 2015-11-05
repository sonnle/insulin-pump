/*
 * BasalDose.c
 *
 *  Created on: Aug 31, 2015
 *      Author: sle
 */

#include "Control.c"
#include "BasalDose.h"

// Global variable
uint32_t BasalDose_DoseAmountCounter;

// Set and enable Timer0 for the basal timer in between doses
void BasalDose_DoseTimingInitiate(void)
{
	// Initialize the pins P1.28 to P1.31
	// The pins are used for debugging
	LPC_GPIO1->FIODIR |= (0xF0000000);
	LPC_GPIO1->FIOPIN &=~(0xF0000000);

	LPC_SC->PCONP |= 1 << 1; // Power up Timer0
	LPC_SC->PCLKSEL0 |= 1 << 2; // Clock select
	LPC_TIM0->PR = 0x02; // Pre-scalar
	LPC_TIM0->MR0 = 1 << 27; // Match number
	LPC_TIM0->MCR |= 1 << 0; // Interrupt and reset
	LPC_TIM0->MCR |= 1 << 1; // Reset timer on Match 0.
	LPC_TIM0->TCR |= 1 << 1; // Manually Reset Timer0 (forced)
	LPC_TIM0->TCR &=~(1 << 1); // Stop resetting the timer.
	NVIC_EnableIRQ(TIMER0_IRQn); // Enable Timer0 IRQ
	LPC_TIM0->TCR |= 1 << 0; // Reset Timer0

	BasalDose_DoseAmountInitiate(); // Call on Timer1
}

// Initialize Timer1
void BasalDose_DoseEnable(void)
{
	LPC_TIM0->IR |= 1 << 0; // Clear MR0 interrupt flag
	LPC_GPIO1->FIOPIN ^= 1 << 29; // Toggle the LED

	NVIC_EnableIRQ(TIMER1_IRQn); // Enable Timer1 IRQ
}

// Set up Timer1 for the speed of the stepper motor
void BasalDose_DoseAmountInitiate(void)
{
	LPC_SC->PCONP |= 1 << 2; // Power up Timer1
	LPC_SC->PCLKSEL0 |= 1 << 2; // Clock select
	LPC_TIM1->PR = 0x02; // Pre-scalar
	LPC_TIM1->MR0 = 1 << 17; // Match number
	LPC_TIM1->MCR |= 1 << 0; // Interrupt and reset
	LPC_TIM1->MCR |= 1 << 1; // Reset timer on Match 0.
	LPC_TIM1->TCR |= 1 << 1; // Manually Reset Timer1 (forced)
	LPC_TIM1->TCR &=~(1 << 1); // Stop resetting the timer.
	LPC_TIM1->TCR |= 1 << 0; // Reset Timer1
}

// Function that calls the stepper motor
void BasalDose_DoseInject(void)
{
	LPC_TIM1->IR |= 1 << 0; // Clear MR0 interrupt flag
	LPC_GPIO1->FIOPIN ^= 1 << 28; // Toggle the LED
	
	// call the stepper motor
	StepperMotor_StepForward();
}
