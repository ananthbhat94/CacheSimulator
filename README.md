# CacheSimulator
This program simulates a Last Level Cache for a new processor that can be used with up to three other processors in a shared memory configuration.

# Compiling and Executing the program
The project consists of 3 header files: HelperFunctions.h CacheFunctions.h MESI.h
3 source files: main.c, HelperFunctions.c, MESI.c
This code was compiled by using a gcc v.6.2
Compile all the 3 source files.
When executing the program, atleast the name of the input trace file has to be given as the 1st argument. The name of of the output file can also be mentioned as the second argument
eg: If the name of the executible is cache, the name of the input is Testcases.din and the name of the output is Output_Testcases.txt, then the commands to compile and run the program will be 
gcc -o cache main.c HelperFunctions.c MESI.c
./cache Testcases.din Output_Testcases.txt

# Cache Details
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

# Authors:
Ananth Bhat
Niraj Thakkar
Shivank Dhote
Sriram VB
