/*
 * Timer.c
 *
 *  Created on: Oct 25, 2015
 *      Author: sle
 */

#include "BasalDose.h"

// Timer0 IRQ Handler
void TIMER0_IRQHandler(void)
{
	if((LPC_TIM0->IR & 0x01) == 0x01) // If MR0 interrupt
	{
		BasalDose_DoseEnable(); // Call to enable Timer1
	}
}

// Timer1 IRQ Handler
void TIMER1_IRQHandler(void)
{
	if((LPC_TIM1->IR & 0x01) == 0x01) // If MRO interrupt
	{
		BasalDose_DoseInject(); // Call for the stepper motor to rotate
	}

}

// External Interrupt
void EINT0_IRQHandler (void)
{
	// Clear interrupt
	LPC_SC->EXTINT = EINT0_CLR;
	
	// Increment counter for number of times EINT0 triggered
	eint0_count++;
	
	// Call for the stepper motor to rotate
	BolusDose_DoseInject();
}