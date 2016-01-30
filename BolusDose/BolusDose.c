/**
 *  BolusDose.c
 *
 *  Created on: Aug 31, 2015
 *      Author: sle
 */

#include "BolusDose.h"
#include "..\Control.h"
#include "..\StepperMotor\StepperMotor.h"

extern STATE Control_GlobalState;
extern STATUS Control_GlobalStatus;
extern REMAINING Control_GlobalRemaining;

extern uint32_t StepperMotor_GlobalPosition;

void BolusDose_DoseInitiate(void)
{
	LPC_PINCON->PINSEL4 &=~ (3<<20); // P2.10 is GPIO
	LPC_GPIO2->FIODIR &=~ (1<<10); // P2.10 in input

	LPC_GPIOINT->IO2IntEnF |= (1 << 10); // Enabling falling edge to P2.10

	NVIC_EnableIRQ(EINT3_IRQn); // Enable External Interrupt 3
}

void EINT3_IRQHandler(void)
{
	LPC_GPIOINT->IO2IntClr |= (1<<10); // Clear the status
	if(StepperMotor_GlobalPosition <= SYRINGE_LENGTH)
	{
		Control_GlobalStatus = Bolus_Status;
		Control_GlobalState = Administration_State;
		LPC_GPIO1->FIOSET |= 1 << 29; // Signal that Bolus is being administered P1.29
		
		if(StepperMotor_GlobalPosition + BOLUS_STEPS > SYRINGE_LENGTH)
		{
			Control_GlobalRemaining = Bolus_Remaining;
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
