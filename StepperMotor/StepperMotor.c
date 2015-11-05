/*
 * StepperMotor.c
 *
 *  Created on: Sep 8, 2015
 *      Author: sle
 */

#include "Control.c"
#include "StepperMotor.h"
#include "BasalDose.h"

// Global variables
uint32_t StepperMotor_CurrentPosition;
uint32_t StepperMotor_GlobalPosition;

// Initializes the stepper motor
void StepperMotor_Initiate(void)
{
	// Initialize the pins P0.0 to P0.3
	// These are the leads for the stepper motor
	LPC_GPIO0->FIODIR |= (0x0000000F);
	LPC_GPIO0->FIOPIN &= ~(0x0000000F);
	
	// Declaring the global variables
	StepperMotor_CurrentPosition = 0;
	StepperMotor_GlobalPosition = 0;
	BasalDose_DoseAmountCounter = 0;
}

// Function that will drive the stepper motor forward
void StepperMotor_StepForward(void)
{
	// Increase the BasalDose_DoseAmountCounter with each step forward
	BasalDose_DoseAmountCounter++;
	
	// Compare and keep track of the current position of the stepper motor
	switch(StepperMotor_CurrentPosition)
	{
		case 0:
			LPC_GPIO0->FIOPIN = (0x00000003);
			StepperMotor_CurrentPosition += 1;
			break;
		case 1:
			LPC_GPIO0->FIOPIN = (0x00000002);
			StepperMotor_CurrentPosition += 1;
			break;
		case 2:
			LPC_GPIO0->FIOPIN = (0x00000006);
			StepperMotor_CurrentPosition += 1;
			break;
		case 3:
			LPC_GPIO0->FIOPIN = (0x00000004);
			StepperMotor_CurrentPosition += 1;
			break;
		case 4:
			LPC_GPIO0->FIOPIN = (0x0000000C);
			StepperMotor_CurrentPosition += 1;
			break;
		case 5:
			LPC_GPIO0->FIOPIN = (0x00000008);
			StepperMotor_CurrentPosition += 1;
			break;
		case 6:
			LPC_GPIO0->FIOPIN = (0x00000009);
			StepperMotor_CurrentPosition += 1;
			break;
		case 7:
			LPC_GPIO0->FIOPIN = (0x00000001);
			StepperMotor_CurrentPosition = 0;
			break;
	}
	
	// Increment stepper motors global variable
	StepperMotor_GlobalPosition += 1;
	
	// Compare if the amount injecfted is more than amount that is able to be recieved
	if(BasalDose_DoseAmountCounter >= STEPS_PER_DOSE)
	{
		NVIC_DisableIRQ(TIMER1_IRQn); // Disable Timer1
		BasalDose_DoseAmountCounter = 0; // Set to 0
	}
}

// Function that will drive the stepper motor backward
void StepperMotor_StepBackward(void)
{
	// Compare and keep track of the current position of the stepper motor
	switch(StepperMotor_CurrentPosition)
	{
		case 0:
			LPC_GPIO0->FIOPIN = (0x00000009);
			StepperMotor_CurrentPosition = 7;
			break;
		case 1:
			LPC_GPIO0->FIOPIN = (0x00000001);
			StepperMotor_CurrentPosition--;
			break;
		case 2:
			LPC_GPIO0->FIOPIN = (0x00000003);
			StepperMotor_CurrentPosition--;
			break;
		case 3:
			LPC_GPIO0->FIOPIN = (0x00000002);
			StepperMotor_CurrentPosition--;
			break;
		case 4:
			LPC_GPIO0->FIOPIN = (0x00000006);
			StepperMotor_CurrentPosition--;
			break;
		case 5:
			LPC_GPIO0->FIOPIN = (0x00000004);
			StepperMotor_CurrentPosition--;
			break;
		case 6:
			LPC_GPIO0->FIOPIN = (0x0000000C);
			StepperMotor_CurrentPosition--;
			break;
		case 7:
			LPC_GPIO0->FIOPIN = (0x00000008);
			StepperMotor_CurrentPosition--;
			break;
	}
	
	// Decrement stepper motors global variable
	StepperMotor_GlobalPosition -= 1;
}
