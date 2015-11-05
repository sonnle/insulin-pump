/*
 * Timer.c
 *
 *  Created on: Oct 25, 2015
 *      Author: sle
 */

#include "BasalDose.c"
#include "BolusDose.h"

// Timer0 IRQ Handler
void TIMER0_IRQHandler(void)
{
	if((LPC_TIM0->IR & 0x01) == 0x01) // If MR0 interrupt
	{
		BasalDose_DoseEnable(); // Call to enable Timer1
	}
}

//Timer1 IRQ Handler
void TIMER1_IRQHandler(void)
{
	if((LPC_TIM1->IR & 0x01) == 0x01) // If MRO interrupt
	{
		BasalDose_DoseInject(); // Call for the stepper motor to rotate
	}
}

// EXternal interrupt 3 Handler
void EINT3_IRQHandler(void)
{
	LPC_GPIOINT->IO2IntClr |= (1<<10); // Clear the status
	LPC_GPIO1->FIOPIN ^= 1 << 31; // Toggle the LED

	NVIC_EnableIRQ(TIMER1_IRQn); // Enable Timer1
}
