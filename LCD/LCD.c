 /**
 *  LCD.c
 *
 *  Created on: Dec 16, 2015
 *      Author: sle
 */

#include <stdio.h>
#include ".\LCD.h"
#include "..\Control.h"
#include "..\InsulinQueue\InsulinQueue.h"
#include "..\StepperMotor\StepperMotor.h"

extern GLCD_FONT GLCD_Font_6x8; // Font size 6x8
extern GLCD_FONT GLCD_Font_16x24; // Font size 16x24

extern STATE Control_GlobalState;
extern STATUS Control_GlobalStatus;

extern uint32_t InsulinQueue_Head;
extern uint32_t *pInsulinQueue_Queue;

extern uint32_t StepperMotor_GlobalPosition;

// Used to hold string representation of our important numbers
char LCD_StringInsulin[20]; 
char LCD_InsulinQueueEntry[10];

void LCD_Initiate(void)
{
	//Initialize the LCD
	GLCD_Initialize();
	
	// Set font size
	GLCD_SetFont(&GLCD_Font_16x24);
	
	// Set background and foreground colors
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	
	// Clear the screen for first use
	GLCD_ClearScreen();
}

void LCD_UpdateScreenStatus()
{
	GLCD_SetFont(&GLCD_Font_16x24);
	switch(Control_GlobalStatus)
	{
		case Basal_Status:
			GLCD_DrawString(20, 20, "Basal\0");
			break;
		case Bolus_Status:
			GLCD_DrawString(20, 20, "Bolus\0");
			break;
		case Backward_Status:
			GLCD_DrawString(20, 20, "Backward\0");
			break;
		case None_Status:
			GLCD_DrawString(20, 20, "None\0");
			break;
		case Wait_Status:
			GLCD_DrawString(20, 20, "Wait\0");
			break;
	}
}

void LCD_UpdateScreenState(void)
{
	GLCD_SetFont(&GLCD_Font_16x24);
	switch(Control_GlobalState)
	{
		case Administration_State:
			GLCD_DrawString(150, 20, "Admin\0");
			break;
		case Empty_State:
			GLCD_DrawString(150, 20, "Empty\0");
			break;
		case Full_State:
			GLCD_DrawString(150, 20, "Full\0");
			break;
		case None_State:
			GLCD_DrawString(150, 20, "Undefined\0");
			break;
	}
}

void LCD_UpdateScreenInsulin(void)
{
	int column, row;
	
	// Format GlobalPosition into string to display on LCD
	GLCD_SetFont(&GLCD_Font_16x24);
	sprintf(LCD_StringInsulin, "%.2f%% Remaining", (1 - (StepperMotor_GlobalPosition / (SYRINGE_LENGTH + 0.0))) * 100);
	GLCD_DrawString(20, 40, LCD_StringInsulin);
	
	// Display current insulin queue on LCD
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_DrawString(10, 140, "Current Insulin Queue\0");
	for(column = 0; column < INSULIN_QUEUE_SIZE / 4; column++)
	{
		for(row = 0; row < 4; row++)
		{
			if(InsulinQueue_Head != 4 * column + row)
			{
				sprintf(LCD_InsulinQueueEntry, "%d", *(pInsulinQueue_Queue + (4 * column + row)));
			}
			else
			{
				sprintf(LCD_InsulinQueueEntry, "*%d", *(pInsulinQueue_Queue + (4 * column + row)));
			}
			GLCD_DrawString((10 + (row * 30)), 150 + (column * 10), LCD_InsulinQueueEntry);
		}
	}
}

void LCD_Basal(void)
{
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_DrawString(20, 20, "Basal Profile\0");
	GLCD_DrawString(20, 40, "Use Arrows for\0");
	GLCD_DrawString(20, 60, "Set Up\0");
	GLCD_DrawString(20, 80, "Up for Low\0");
	GLCD_DrawString(20, 105, "Right for Medium\0");
	GLCD_DrawString(20, 130, "Down for High");
}

void LCD_Bolus(void)
{
	
}
