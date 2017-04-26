/*
This file has 1 function which implements the MESI protocol to maintain Cache Coherency. It takes the following inputs:
-event:		The event number from the trace file
-index:		The current index of the cache line
-way:		The current way of the cache line 
-address:	The address given in the trace file
-*SnoopResponse:This pointer variable is used to update the result of our snooping bus operations by other caches
-*Busop:	This pointer variable is used to simulate Bus Operations 
-*GetSnoop:	Used to simulate the reporting of snoop results by other caches
-*Status:	This pointer variable helps indicating whether a line in the higher level cache needs to be evicted

Note: 
-This function does not return any arguments
-This function will write error messages to a file. 

Version:1.0

Authors:
-Ananth Bhat
-Niraj Thakkar
-Shivank Dhote
-Sriram VB
*/

#include <stdio.h>
#include <stdlib.h>
#include "CacheFunctions.h"
#include "MESI.h"
struct cacheline SimCache[INDEX][WAY];

void MESI (int event, int index, int way, unsigned int address, int *SnoopResponse, int *BusOp, int* GetSnoop, int* status)
{
    switch(SimCache[index][way].MESI)
    {
        case M :    if((event == PROCREAD) || (event == PROCWRITE) ||  (event == PROCINSTRREAD))
                    {
                        SimCache[index][way].MESI = M;
                        *BusOp = NOBUSOP;
			*status =0;
			*GetSnoop = GetSnoopResult(address);
                    }

                    else if((event == SNOOPREAD))
                    {
                        *SnoopResponse = HITM;
                        *BusOp = WRITE;
                        SimCache[index][way].MESI = S;
			*status =0;
                    }
                    else if((event == SNOOPRWIM))
                    {
                        *SnoopResponse = HITM;
                        *BusOp = WRITE;
                        SimCache[index][way].MESI = I;
			*status =1;
                    }
                    else
		    {
			*status =0;
			*BusOp = NOBUSOP;
			fprintf(outfp,"\n Error: Invalid Transition \n");
		    }
                        
                    break;

        case E :    if((event == PROCREAD )|| (event == PROCINSTRREAD))
                    {
                        SimCache[index][way].MESI = E;
                        *BusOp = NOBUSOP;
			*status =0;
			*GetSnoop = GetSnoopResult(address);
                    }
                    else if(event == PROCWRITE)
                    {
                        SimCache[index][way].MESI = M;
                        *BusOp = NOBUSOP;
			*status =0;
			*GetSnoop = GetSnoopResult(address);
                    }

                    else if(event == SNOOPREAD)
                    {
                        *SnoopResponse = HIT;
                        SimCache[index][way].MESI = S;
                        *BusOp = NOBUSOP;
			*status =0;
                    }
                    else if(event == SNOOPRWIM)
                    {
                        SimCache[index][way].MESI = I;
                        *BusOp = NOBUSOP;
                        *SnoopResponse = NOHIT;
			*status =1;
                    }
                    else
		    {
			*status =0;
			*BusOp = NOBUSOP;
			fprintf(outfp,"\n Error: Invalid Transition \n");
		    }
                    break;

        case S :    if((event == PROCREAD) || (event == PROCINSTRREAD))
                    {
                        SimCache[index][way].MESI = S;
                        *BusOp = NOBUSOP;
			*status =0;
			*GetSnoop = GetSnoopResult(address);
                    }

                     else if(event == PROCWRITE)
                     {
                        *BusOp = INVALIDATE;
                        SimCache[index][way].MESI = M;
			*status =0;
			*GetSnoop = GetSnoopResult(address);
                     }
                     else if(event == SNOOPREAD)
                     {
                        *SnoopResponse = HIT;
                        SimCache[index][way].MESI = S;
                        *BusOp = NOBUSOP;
			*status =0;
                     }
                     else if((event == SNOOPINV) || (event == SNOOPRWIM))
                     {
                         SimCache[index][way].MESI = I;
			 *SnoopResponse = NOHIT;
			 *status =1;
			 *BusOp = NOBUSOP;
                     }

                     else
                    {
			*status =0;
			fprintf(outfp,"\n Error: Invalid Transition \n");
		    }
                
                    break;

        case I :    if((event == PROCREAD) || (event == PROCINSTRREAD))
                     {
                        *BusOp = READ;
                        *GetSnoop = GetSnoopResult(address);
                        if((*GetSnoop == HIT )|| (*GetSnoop == HITM))
                            SimCache[index][way].MESI = S;
                        else
                            SimCache[index][way].MESI = E;
			*status =0;
                     }
                    else if(event == PROCWRITE)
                     {
                        *BusOp = RWIM;
                        SimCache[index][way].MESI = M;
			*GetSnoop = GetSnoopResult(address);
			*status =0;
                     }
                    else if((event == SNOOPREAD )|| (event == SNOOPINV )|| (event == SNOOPWRITE )|| (event == SNOOPRWIM))
                     {
                        *SnoopResponse = NOHIT;
                        SimCache[index][way].MESI = I;
                        *BusOp = NOBUSOP;
			*status =0;
			*BusOp = NOBUSOP;
                     }
                    else
                    {
			*status =0;
			*BusOp = NOBUSOP;
			fprintf(outfp,"\n Error: Invalid Transition \n");
		    }
                    
                    break;

        default:    fprintf(outfp,"error state \n");
		    *status =0;
		    *BusOp = NOBUSOP;
                  
                    break;

    }

}
