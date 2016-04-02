 /**
 *  LCD.c
 *
 *  Created on: Dec 16, 2015
 *      Author: sle
 */
 
#include <stdio.h>
#include ".\LCD.h"
#include <stdint.h>
#include "..\Control.h"
#include "Board_GLCD.h"  // Board Support : Graphic LCD
#include "GLCD_Config.h" // Board Support : Graphic LCD
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
char LCD_ADCReading[20];
char LCD_PHReading[20];

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
	sprintf(LCD_StringInsulin, "%d%% Remaining", 100 - ((StepperMotor_GlobalPosition * 100) / SYRINGE_LENGTH));
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

void LCD_DisplayOptions(BaseDisplay current)
{
	LCD_SetUpCommon();
	
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_DrawString(186, 30, current.ProfileCategory);
	
	GLCD_DrawString(140-((current.Size1 + 3)*6), 121, current.ProfileOption1);
	
	GLCD_DrawString(204, 121, current.ProfileOption2);
	
	GLCD_DrawString(160 - ((current.Size3 / 2) * 6), 85, current.ProfileOption3);
	
	GLCD_DrawString(160 - ((current.Size4 / 2) * 6), 160, current.ProfileOption4);
	
	GLCD_DrawString(90, 185, "Push Down: Glucose Reading\0");
}

void LCD_ClearScreen(void)
{
	GLCD_ClearScreen();
}

// bitmap taken from Demo.c from Keil uVision example package
const uint8_t Arrows_16bpp_red[] = {
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6b,0x6d,0x6b,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6b,
  0xff,0xff,0xff,0xff,0x6d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0x6d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0x8e,0x73,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0x8e,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0x8e,0x73,0x8e,0x73,0xae,0x73,0xae,0x73,0xae,0x73,
  0xae,0x73,0xae,0x7b,0xcf,0x7b,0xcf,0x7b,0xcf,0x7b,0xcf,0x7b,0xff,0xff,0x6d,0x6b,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xef,0x7b,0x6d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7b,0x6d,0x6b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7b,
  0x6d,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xef,0x83,0xff,0xff,0x8e,0x73,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xae,0x73,0xae,0x7b,0xcf,0x7b,0xcf,0x7b,0xcf,0x7b,0xcf,0x7b,
  0xcf,0x7b,0xef,0x7b,0xef,0x7b,0xef,0x7b,0xef,0x83,0xff,0xff,0xff,0xff,0xff,0xff,
  0x8e,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xae,0x7b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xae,0x73,0xff,0xff,0xff,0xff,0xff,0xff,
  0xcf,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xae,0x73,0xff,0xff,0xff,0xff,0xcf,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x7b,0xcf,0x7b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0x92,0x94,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0x92,0x94,0xff,0xff,0xff,0xff,0xb2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xb2,0x94,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xd2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0x51,0x8c,
  0x71,0x8c,0x71,0x8c,0x71,0x8c,0x71,0x94,0x92,0x94,0x92,0x94,0x92,0x94,0x92,0x94,
  0x92,0x94,0xb2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd3,0x9c,
  0xff,0xff,0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xd3,0x9c,0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0x71,0x8c,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xf3,0x9c,0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0xff,0xff,0x91,0x94,0x92,0x94,
  0x92,0x94,0x92,0x94,0xb2,0x94,0xb2,0x94,0xb2,0x94,0xb2,0x94,0xb2,0x94,0xb2,0x94,
  0xd3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xd3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xf3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd3,0x9c,0xff,0xff,
  0xff,0xff,0xff,0xff,0xf3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xd3,0x9c,0xff,0xff,0xff,0xff,0xf3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0xf3,0x9c,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0x71,0x8c,0x71,0x8c,0x71,0x8c,0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x71,0x8c,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x71,0x94,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xb2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb2,0x94,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb2,0x94,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xb2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xd2,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x92,0x94,
  0x92,0x94,0xb2,0x94,0xb2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd2,0x9c,
  0xd3,0x9c,0xd3,0x9c,0xd3,0x9c,0xff,0xff,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xf3,0x9c,0x92,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,
  0xff,0xff,0xb2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,
  0xb2,0x94,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xf3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd2,0x9c,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd3,0x9c,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf3,0x9c,0xf3,0x9c,0xf3,0x9c,
  0xf3,0x9c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6b,0x6d,0x6b,0x6d,0x6b,0x6d,0x73,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0x6d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8e,0x73,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8e,0x73,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0x6d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xae,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0x6d,0x6b,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xae,0x73,0xff,0xff,0x6d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xcf,0x7b,0x6d,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x7b,
  0xff,0xff,0x6d,0x6b,0x8e,0x73,0x8e,0x73,0x8e,0x73,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xae,0x73,0xae,0x7b,0xcf,0x7b,0xcf,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0x8e,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xae,0x73,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xae,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x7b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xae,0x73,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xae,0x73,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xcf,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xae,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x7b,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xce,0x73,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x7b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x7b,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xef,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xcf,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7b,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xcf,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7b,
  0xef,0x7b,0xef,0x7b,0xef,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x10,0x84,0x10,0x84,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7b,
  0xf0,0x83,0xff,0xff,0xff,0xff,0x10,0x84,0x10,0x84,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xef,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0x30,0x84,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7b,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x30,0x84,0xff,0xff,
  0xff,0xff,0x0f,0x84,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0x51,0x8c,0xff,0xff,0x10,0x84,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x51,0x8c,
  0x10,0x84,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0x51,0x8c,0xff,0xff,0x10,0x84,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x51,0x8c,0xff,0xff,
  0xff,0xff,0x10,0x84,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,0x30,0x84,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x51,0x8c,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0x30,0x8c,0x51,0x8c,0xff,0xff,0xff,0xff,0x51,0x8c,
  0x71,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0x51,0x8c,0x51,0x8c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
};

void LCD_SetUpCommon(void)
{
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_DrawString(112, 5, "Set-Up\0");
	GLCD_DrawBitmap(160, 120, 12, 12, &Arrows_16bpp_red[4*532]); // center dot
	GLCD_DrawBitmap(159, 100, 14, 19, &Arrows_16bpp_red[2*532]); // up arrow
	GLCD_DrawBitmap(159, 133, 14, 19, &Arrows_16bpp_red[3*532]); // down arrow
	GLCD_DrawBitmap(140, 119, 19, 14, &Arrows_16bpp_red[0*532]); // left arrow
	GLCD_DrawBitmap(173, 119, 19, 14, &Arrows_16bpp_red[1*532]); // right arrow
	GLCD_SetFont(&GLCD_Font_6x8);
	GLCD_DrawString(1, 30, "Select option on joystick for:\0");
}

void LCD_DisplayADC(ProfileOptions* current)
{
	sprintf(LCD_ADCReading, "ADC: %d", current->LastADCReading);
	GLCD_DrawString(20, 100, LCD_ADCReading);
	
	sprintf(LCD_PHReading, " PH: %.2f", current->LastPHReading);
	GLCD_DrawString(20, 110, LCD_PHReading);
}
