/*
 * BolusDose.c
 *
 *  Created on: Aug 31, 2015
 *      Author: sle
 */
 
#include "Control.c"
#include "BolusDose.h"

// Set and enable External Interrupt 3
void BolusDose_DoseInitiate(void)
{

	LPC_PINCON->PINSEL4 &=~ (3<<20); // P2.10 is GPIO
	LPC_GPIO2->FIODIR &=~ (1<<10); // P2.10 in input

	LPC_GPIOINT->IO2IntEnF |= (1 << 10); // Enabling falling edge to P2.10

	NVIC_EnableIRQ(EINT3_IRQn); // Enable External Interrupt 3
}
