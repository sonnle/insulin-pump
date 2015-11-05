/*
 * Control.c
 *
 *  Created on: Aug 31, 2015
 *      Author: sle
 */

#include "lpc17xx.h"                 // Device header
#include "StepperMotor.h"
#include "BasalDose.h"
#include "BolusDose.h"

// Global variables declared in other files
extern uint32_t StepperMotor_GlobalPosition;
extern uint32_t BasalDose_DoseAmountCounter;

// Create bool
typedef enum {false, true} bool;

#define MAX_TIME 75000
#define STEPS_PER_DOSE 50

// Global variables
uint32_t i = 0;
uint32_t wait = MAX_TIME;

bool Control_IsSyringeEmpty(void);

int main(void)
{
	SystemInit(); // Initialize the device
	
	StepperMotor_Initiate(); // Initialize the stepper motor
	BasalDose_DoseTimingInitiate(); // Initialize Timer0
	BolusDose_DoseInitiate(); // Initialize External Interrupt 3

	while(1);
}

// Test if the syringe is empty
bool Control_IsSyringeEmpty()
{
	if(StepperMotor_GlobalPosition >= 20)
		return true;
	else
		return false;
}

