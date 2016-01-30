/**
 *  BasalDose.c
 *
 *  Created on: Aug 31, 2015
 *      Author: sle
 */

#include "BasalDose.h"
#include "..\Control.h"
#include "..\StepperMotor\StepperMotor.h"

extern STATE Control_GlobalState;
extern STATUS Control_GlobalStatus;
extern REMAINING Control_GlobalRemaining;

extern uint32_t StepperMotor_GlobalPosition;

// Set and enable Timer0 for the time in between Basal doses
void BasalDose_TimingInitiate(void)
{
	LPC_TIM0->PR = 0x02; // Pre-scalar
	LPC_TIM0->MR0 = 1 << 27; // Match number
	LPC_TIM0->MCR |= 3 << 0; // Interrupt and reset timer on match (MCR = 011)
	NVIC_EnableIRQ(TIMER0_IRQn); // Enable Timer0, but don't start counting
}

void BasalDose_TimingEnable(void)
{
	StepperMotor_SpinDisable(); // Disable and Reset Timer1
	LPC_TIM0->TCR |= 1 << 0; // Start counting (TCR = 01)
}

void BasalDose_TimingDisable(void)
{
	LPC_TIM0->TCR &=~(1 << 0); // Stop Timer Counter (TCR = 00)
	LPC_TIM0->TCR |= 1 << 1; // Reset Timer Counter (TCR = 10)
	LPC_TIM0->TCR &=~(1 << 1); // Stop resetting Timer Counter (TCR = 00)
	LPC_TIM0->IR |= 1 << 0; // Reset Timer0 Interrupt
}

void TIMER0_IRQHandler(void)
{
	if(StepperMotor_GlobalPosition <= SYRINGE_LENGTH)
	{
		Control_GlobalStatus = Basal_Status;
		Control_GlobalState = Administration_State;
		LPC_GPIO1->FIOSET |= 1 << 28; // Signal that Basal is being administered P1.28
		
		if(StepperMotor_GlobalPosition + BASAL_STEPS > SYRINGE_LENGTH)
		{
			Control_GlobalRemaining = Basal_Remaining;
		}
		else
		{
			Control_GlobalRemaining = None_Remaining;
		}
	}
	else
	{
		Control_GlobalStatus = None_Status;
		Control_GlobalState = Empty_State;
		LPC_GPIO2->FIOSET |= 1 << 2; // Signal that syringe is empty P2.2
	}
	StepperMotor_SpinEnable();	
}
