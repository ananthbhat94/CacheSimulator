/*
This is the header file for HelperFunctions.c
All function definitions are listed below.

Version:1.0

Authors:
-Ananth Bhat
-Niraj Thakkar
-Shivank Dhote
-Sriram VB

*/

#ifndef _HELPER_FUNCTIONS
#define _HELPER_FUNCTIONS

extern int IndexBits;
extern int ByteSel;

extern FILE *outfp;

char* print_string (int num, int type);

void PrintStates (int BusOp, int SnoopResponse, int GetSnoop, int event, int index, int way);

void ClearCache(void);

int CheckTagBits(int index, int tag, int *hit);

void CacheDisplay(void);

int IndexMask (unsigned int address);

int GetSnoopResult (unsigned int address);

void updateLRU(int index, int way);

int GetLRU(int index, int*BusOp);

void Message(int address, int status);

void PrintStats(int read, int write, int hit, int miss);

#endif
