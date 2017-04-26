/**************************************************************************
This file contains all Helper Functions required for the Cache Simulations.
The description of each function is given above the function definition.

Version:1.0

Authors:
-Ananth Bhat
-Niraj Thakkar
-Shivank Dhote
-Sriram VB

****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "CacheFunctions.h"
#include "HelperFunctions.h"
int IndexBits = log2(INDEX);
int ByteSel = log2 (LINESIZE);

extern struct cacheline SimCache[INDEX][WAY];

/*****Print String*****
This function returns the names of 
It takes 2 arguments:
@param: num:	Number denotes the value in a particular type
@param: type:	Type denotes one of the following types
Type:
0 : MESI
1 : Bus OP Types
2 : Snoop Result Types
3 : Event types 

Return type is a character pointer
*/
char* print_string (int num, int type)
{
switch (type)
{
	case 0: switch(num)
	{
		case M: 		return (char*) "Modified";

		case E: 		return (char*) "Exclusive";

		case S: 		return (char*) "Shared";

		case I: 		return (char*) "Invalid";

		default:		return (char*) "Error";
	}

	break;

	case 1: switch(num)
	{
		case READ:      	return (char*) "Read";

		case WRITE:     	return (char*) "Write";

		case INVALIDATE:	return (char*) "Invalidate";

		case RWIM:      	return (char*) "RWIM";

		case NOBUSOP:   	return (char*) "No Bus OP";

		default:		return (char*) "Error";
	}

	break;

	case 2: switch(num)
	{
		case NOHIT: 		return (char*) "No Hit";

		case HIT:   		return (char*) "Hit";

		case HITM:  		return (char*) "Hit to a Modified Line";

		default:    		return (char*) "Error";
	}

	break;

	case 3: switch(num)
	{
		case PROCREAD: 		return (char*) "Processor Data Read";

		case PROCWRITE: 	return (char*) "Processor Write";

		case PROCINSTRREAD: 	return (char*) "Processor Instruction Read";

		case SNOOPINV: 		return (char*) "Snooped Invalidate";

		case SNOOPREAD: 	return (char*) "Snooped Read";

		case SNOOPWRITE: 	return (char*) "Snooped Write";

		case SNOOPRWIM: 	return (char*) "Snooped RWIM";

		case RESET: 		return (char*) "Reset";

		case PRINTALL: 		return (char*) "Print All";

		default:		return (char*) "Error";
	}
	break;

	default: 			return "Error";

	}

	return 0;
}
/*****Print States*****
This function is used to Print the state of a selected cache line
*/

void PrintStates (int BusOp, int SnoopResponse, int GetSnoop, int event, int index, int way)
{
    
    if(event == PROCINSTRREAD || event == PROCREAD || event == PROCWRITE)
    {
        if(BusOp != NOBUSOP)
        {
            fprintf(outfp,"BUS OP: %s\n", print_string(BusOp,1));
            
        }
	fprintf(outfp,"Received Snoop Response: %s\n", print_string(GetSnoop,2));
        fprintf(outfp, "Modified State: %s\n", print_string(SimCache[index][way].MESI,0));
    }
    else if(event == SNOOPINV || event == SNOOPREAD || event == SNOOPWRITE || event == SNOOPRWIM)
    {
        fprintf(outfp,"Put Snoop Result: %s\n", print_string(SnoopResponse, 2));
        if(BusOp != NOBUSOP)
            fprintf(outfp,"BUS OP: %s\n", print_string(BusOp, 1));
        fprintf(outfp,"Modified State: %s\n", print_string(SimCache[index][way].MESI,0));
    }
  
}

/*****Clear Cache Lines*****
This function clears all the cache lines
It takes no arguments and returns no arguments
*/
void ClearCache(void)
{
  
  int index;
  int way;
  fprintf(outfp, "\n**********Clearing Cache**********\n");
  for (index = 0; index <= INDEX; index++)
  {
    for (way = 0; way < WAY; way++)
    {
       SimCache[index][way].MESI = I;
       SimCache[index][way].LRU = WAY -1;
    }
  }
}

/****Check Tag bits*****
This function checks the tag bits to determine whether the memory reference is
a hit or a miss in the given index. It does this by checking the tag bits of all 
the ways of the particular index and comparing it with tag bits embedded in the 
address. If its a hit, it returns the way number. However, if its a miss, it returns
an error code which is always (WAY+1), ie. 1 greater than the number of total WAYs
This error code can then be checked to determine if there was a miss.
*/
int CheckTagBits(int index, int tag, int *hit)
{

  int way;
  for (way = 0; way < WAY; way++)
  {
    if ((SimCache[index][way].TAG == tag) && (SimCache[index][way].MESI != I))
    {
        fprintf(outfp, "\n**********HIT**********\n");
        (*hit)++;
        return way;
    }
  }
  return (WAY+1);//WAY +1 indicates an error
}

/*****Display Cache Lines*****
Displays state of all the valid cache lines in the cache 
*/
void CacheDisplay(void)
{
   fprintf(outfp, "\n**********Display Cache Lines********\n");
   int index;
   int way;
   int flag =0;
   for (index = 0; index < INDEX; index++)
   {
       for (way = 0; way < WAY; way++)
      {
        if ((SimCache[index][way].MESI != I) && flag == 0)
        {
            fprintf(outfp,"INDEX: %d\n",index);
            flag = 1;
        }
        if(SimCache[index][way].MESI != I)
        {
           fprintf(outfp,"WAY: %d \t LRU: %d \t MESI: %s \t TAG: %x\n", \
                way, SimCache[index][way].LRU, print_string(SimCache[index][way].MESI,0), SimCache[index][way].TAG);
            
        }
      }
        flag = 0;
    }
}
/*****Index Mask*****
This function decodes which index is being referred to from the
given address and returns the index number
*/
int IndexMask (unsigned int address)
{
    int i;
    int mask = 1;
    int index;
    address = address >> ByteSel;
    for(i=0;i<IndexBits;i++)
    {
        mask = mask *2;
    }
    index = address & (mask-1);
    return index;
}

/*****Get Snoop Result*****
This function is used to simulate the snooping of other processors.
Whenever the cache needs the snoop response of other processors, it will 
call this function. 
This function returns a random but predictable snoop result. To aachieve
this, the snoopresult is based on the combination of the 2 lowest significant
bits of the address. 
*/

int GetSnoopResult (unsigned int address)
{
    int snoopbits = (address & SNOOPMSK);
    int snoopresponse;
    switch (snoopbits)
    {
        case CASE0 :    snoopresponse = NOHIT;
                        break;
        case CASE1 :    snoopresponse = HIT;
                        break;
        case CASE2 :    snoopresponse = HITM;
                        break;
        default:        snoopresponse = NOHIT;
                        break;
    }
    return snoopresponse;
}

/*****Update LRU*****
Updates the LRU of a selected Cache line
The function takes the index and way number of
the most recently used line as inputs and correspondingly
updates the LRU bits of all the cachelines
*/
void updateLRU(int index, int way)
{
     // if the LRU bits of our way are already the most recently used, we do nothing
     if (SimCache[index][way].LRU == 0)
        return;
     else
     {
        int ourbits = SimCache[index][way].LRU;
        int testway;

        for(testway = 0; testway <= WAY - 1; testway++)
        {
        	if(SimCache[index][testway].LRU < ourbits )
        		SimCache[index][testway].LRU++;
        }
      }

        SimCache[index][way].LRU = 0;
}

/*****Get LRU*****
This function will return the way number of either an invalid way, 
or the least recently used way of a particular index.
This function is used whenever there is a cache miss. If a modified line
is being evicted, then a Writeback (WRITE) is issued
*/
int GetLRU(int index, int*BusOp)
{
    int way;
	for(way = 0; way <= WAY - 1 ; way++)
	{
		if(SimCache[index][way].MESI == I)
			return(way);
	}
	for(way = 0; way <= WAY - 1 ; way++)
	{
		if(SimCache[index][way].LRU == WAY - 1)
		{
		    if(SimCache[index][way].MESI == M)
		        *BusOp = WRITE;
		    return(way);
		}

	}
}

/******Message to higher level cache*****
This function sends a message to a higher level cache only if a line has to be 
evicted from the higher level cache. This is determined by the 'status' variable 
which is controlled in the function "MESI" in the file MESI.c
*/
void Message(int address, int status)
{
    
	if(status)
		fprintf(outfp, "Message to Higher Level Cache: Invalidate Address = %x \n", address);
    
}

/*****Prints Cache Statistics*****
This function prints the cache statistics. The following cache statistics are printed:
-Number of Reads
-Number of Writes
-Number of Hits
-Number of Misses
-Hit Ratio
*/
void PrintStats(int read, int write, int hit, int miss)
{
    float hit_ratio = ((float)(hit))/((float)(hit+miss));
    fprintf(outfp, "\n**********Printing Cache Statistics**********");
    fprintf(outfp, "\nNumber of Cache Reads: %d \nNumber of Cache Writes: %d \nNumber of hits: %d \nNumber of Misses: %d \nHit Ratio: %f \n"\
            ,read,write,hit,miss,hit_ratio);
    printf("\nNumber of Cache Reads: %d \nNumber of Cache Writes: %d \nNumber of hits: %d \nNumber of Misses: %d \nHit Ratio: %f\n"\
            ,read,write,hit,miss,hit_ratio);
    
}
