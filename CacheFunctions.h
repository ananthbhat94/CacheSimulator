/*
This file contains all the definitions for a cache
The Capacity, Linesize and the number of Ways can be changed during compilation

Version:1.0

Authors:
-Ananth Bhat
-Niraj Thakkar
-Shivank Dhote
-Sriram VB

*/

#ifndef _CACHE_FUNCTIONS
#define _CACHE_FUNCTIONS

#include <math.h>

//Cache Property Definitions

#ifndef CAPACITY
#define     CAPACITY    16*1024*1024
#endif

#ifndef LINESIZE 
#define     LINESIZE    64
#endif

#ifndef WAY
#define     WAY         8
#endif

#define     INDEX       CAPACITY/(WAY*LINESIZE)


//MESI Definitions

#define     M           0
#define     E           1
#define     S           2
#define     I           3


//Bus Operation types

#define     READ            1 /* Bus Read */
#define     WRITE           2 /* Bus Write */
#define     INVALIDATE      3 /* Bus Invalidate */
#define     RWIM            4 /* Bus Read With Intent to Modify */
#define     NOBUSOP         5 /* No Bus Operation */

//Snoop Result types

#define     NOHIT           0 /* No hit */
#define     HIT             1 /* Hit */
#define     HITM            2 /* Hit to modified line */

//TRACE requests

#define     PROCREAD        0 //Processor Data Read
#define     PROCWRITE       1 //Processor Data Write
#define     PROCINSTRREAD   2 //Processor Instruction Read
#define     SNOOPINV        3 //Snooped Invalidate
#define     SNOOPREAD       4 //Snooped Read
#define     SNOOPWRITE      5 //Snooped Write
#define     SNOOPRWIM       6 //Snooped Read With Intent to Modify
#define     RESET           8 //Reset 
#define     PRINTALL        9 //Print all valid cachelines


//Mask Definitions
#define     SNOOPMSK        3 
#define     CASE0           0
#define     CASE1           1
#define     CASE2           2

// structure for the status of a cache line
struct cacheline
{
	int MESI;
	int LRU;
	int TAG;
};

#endif
