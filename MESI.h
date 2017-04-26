/*
This is the header file for MESI.c

Version:1.0

Authors:
-Ananth Bhat
-Niraj Thakkar
-Shivank Dhote
-Sriram VB
*/

#ifndef _MESI
#define _MESI

#include "HelperFunctions.h"

void MESI (int event, int index, int way, unsigned int address, int *SnoopResponse, int *BusOp, int* GetSnoop, int* status);

#endif
