/************************************************************************************************
This is the main program which contains the main function.
This program simulates a Last Level Cache for a new processor that	
can be used with up to three other processors in a shared memory configuration.

The cache has a total capacity of 16MB, uses 64-byte lines, and is 8-way set associative. It
employs a write allocate policy and uses the MESI protocol to ensure cache coherence. The
replacement policy is implemented with a true-LRU scheme. The system maintains inclusivity.

The following cache properties can the changed by redefining the macros
-CAPACITY
-LINESIZE
-WAY (Number of ways)
during compilation

There is no data storage in the cache as the cache behavior (hits, misses, evictions, etc)
is independent of the data.
			

Version:1.0

Authors:
-Ananth Bhat
-Niraj Thakkar
-Shivank Dhote
-Sriram VB
***********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "HelperFunctions.h"
#include "MESI.h"
#include "CacheFunctions.h"

//Definitions
int SnoopResponse;
int BusOp;//Stores the Bus operations of the current trace execution 
int GetSnoop;//Stores snoop result of other processors in the system
int rd = 0; //counts number of reads
int wr =0; //counts number of writes
int miss =0; //counts number of misses
int hit =0; //counts number of hits
FILE *outfp; //Output file pointer
int status =0; //Status flag indicates when to send message to higher level cache. Needed to maintain inclusivity
FILE *ifp;//Input File pointer
struct cacheline SimCache[INDEX][WAY];//A structure representing the cache and its properties 
//Define 2 char pointers to take input and output filenames
char *input;
char *output;
//Main Function
int main(int argc, char * argv[])
{
	//Input and output file names are taken as command line arguments.
	//No file names are mentioned. Default file names are taken
	if(argc == 1)
	{
		printf("Input and Output File names not mentioned\nUsing default input and output files: Test.din and Output.txt\n");
		input = "Faust.din";
		output = "Output.txt";
	}
	//Only input file name is mentioned. Default file name Output.txt is created
	else if (argc == 2)
	{
		printf("Output file not mentioned. The Output file will be named Output.txt\n");
		input = argv[1];
		output = "Output.txt";
	}
	//Both input and output file names are mentioned
	else
	{
		input = argv[1];
		output = argv[2];
	}

	ifp = fopen(input, "r"); //Open Input File
	outfp = fopen(output, "w");//Open output file in 'Write' mode
	
	//Variable Definitions
	int k;
	int i=0;
	int event,address;

	ClearCache();//Clear cache on startup
	
	//Scan the input file for each trace instruction and accordingly process them 
	while ((fscanf(ifp, "%d %x\n", &event, &address)!= EOF))
	{
		//Detect and report invalid event
		if((event == 7) || (event > 9))
		{
			fprintf(outfp, "Invalid Event\n");
			continue;
		}
		//Detect reads
		else if(event == PROCREAD || event == PROCINSTRREAD)
			rd++;
		//Detect writes
		else if(event == PROCWRITE)
			wr++;
		//Detect reset event
		else if(event == RESET)
		{
			ClearCache();
			continue;
		}
		//Detect cache display event 
		else if(event == PRINTALL)
		{
			CacheDisplay();
			continue;
		}

		//Calculate Cache Properties
		int index = +IndexMask(address);
		int tagshift = ByteSel + IndexBits;
		int tag = address >> tagshift;
		int way = CheckTagBits(index, tag, &hit);

		//Check if Cache Access is a Miss
		if(way == (WAY +1))
		{
			fprintf(outfp, "\n**********MISS**********\n");
			miss++;
			way = GetLRU(index, &BusOp);
			SimCache[index][way].MESI = I;
		}
		
		//Assign tag calculated from the given address to the cache line
		SimCache[index][way].TAG = tag;

		/**********************Print Statements******************************/

		//Print Address
		fprintf(outfp, "ADDRESS = %x\n", address);
		//Print Event
		fprintf(outfp, "EVENT = %d \t %s\n", event, print_string(event, 3));
		//Print Way and Index
		fprintf (outfp, "The WAY is %d and the INDEX is %d\n", way, index);
		//Print Current Iteration Number
		fprintf(outfp, "Iteration : %d \n",i);
		//printf("Iteration : %d \n",i);
		//Print Current State
		fprintf(outfp,"Current State: %s\n", print_string(SimCache[index][way].MESI,0));

		//Run through MESI protocol
		MESI(event, index, way, address, &SnoopResponse, &BusOp, &GetSnoop, &status);
		
		//Print result of each trace request
		PrintStates(BusOp, SnoopResponse, GetSnoop, event, index, way );
		
		//Send message to higher level cache
		if(BusOp != NOBUSOP)
			Message(address, status);
		
		/****Print LRU Status Before and after exectuting Trace Instruction*****/
		fprintf(outfp, "LRU of current Index before updating:\t");
		for(k=0;k<WAY;k++)
			fprintf(outfp, "%d \t", SimCache[index][k].LRU);
		updateLRU(index,way);
		fprintf(outfp, "LRU of current Index after updating:\t");
		for(k=0;k<WAY;k++)
			fprintf(outfp, "%d \t", SimCache[index][k].LRU);
		fprintf(outfp,"\n");

		i++;//Update Iteration number
		BusOp = NOBUSOP;//Reset Bus operation to No Bus Operation

	}

	/*Print Cache Statistic after execution of entire trace file. 
	Print Number of Reads
	      Number of Writes
	      Number of Hits
	      Number of Misses
	      Hit Ratio
	*/
	PrintStats(rd,wr,hit,miss);
	
	fclose(ifp);//Close input file
	fclose(outfp);//Close output file

}
