/*
 * BolusDose.h
 *
 *  Created on: Aug 31, 2015
 *      Author: sle
 */

/** Function:  BolusDose_DoseInitiate()
 * 
 * Must be called at the beggining of the main method.
 * Fucntion is used to set pin 2.10 (the INT0 button) for an input.
 * The interrupt is set up for falling edge.
 */

void BolusDose_DoseInitiate(void);
