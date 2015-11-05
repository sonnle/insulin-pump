/*
 * StepperMotor.h
 *
 *  Created on: Sep 8, 2015
 *      Author: sle
 */

#ifndef STEPPERMOTOR_STEPPERMOTOR_H_
#define STEPPERMOTOR_STEPPERMOTOR_H_

/* Function: StepperMotor_Initiate()
 *
 * Must be called at the beginning of the main method.
 * Function is used to set the GPIO0 pins 0, 1, 2, 3 to output,
 * then masks bits 4,...,31 to maintain integrity of those pins.
 * Currently, sets StepperMotor_GlobalPosition and StepperMotor_CurrentPosition to 0, which are
 * variables that describe where the stepper motor is relative
 * from where it starts.
 */
 
void StepperMotor_Initiate(void);

/* Function: StepperMotor_StepForward()
 *
 * Called when Timer1 is enabled.
 * Function is used to make half steps on the stepper motor.
 * Currently, adds StepperMotor_GlobalPosition and StepperMotor_CurrentPosition by 1 with each half step.
 */
 
void StepperMotor_StepFoward(void);

/* Function: StepperMotor_StepBackward()
 *
 * Called when the Control_IsSyringeEmpty() is true.
 * Function makes half steps on the stepper motor.
 * Currently, subtracts StepperMotor_GlobalPosition and StepperMotor_CurrentPosition by 1 with each half step.
 */
void StepperMotor_StepBackward(void);

#endif /* STEPPERMOTOR_STEPPERMOTOR_H_ */
