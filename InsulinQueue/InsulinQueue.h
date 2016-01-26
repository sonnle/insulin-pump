/**
 *  InsulinQueue.h
 *
 *  Created on: Dec 5, 2015
 *      Author: sle
 */
#include <stdlib.h>
#include "..\Control.h"

typedef struct InsulinQueue
{
	uint32_t maxNumEntries;
	uint32_t curNumEntries;
	int head;
	int tail;
	uint32_t *insulinEntry;
} InsulinQueue;

void InsulinQueue_Initiate(void);

InsulinQueue *InsulinQueue_Create(uint32_t maxEntries);

void InsulinQueue_Pop(InsulinQueue *iq);

void InsulinQueue_Push(InsulinQueue *iq, int currentInsulinAmount);
