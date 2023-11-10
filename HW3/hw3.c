// Alex Groves
// Homework 3, CSCI 340-01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // for memset()

int main(int argc, char *argv[])
{
	if ( argc < 2 ) {	// user stuff
		fprintf(stderr, "usage: hw3 <memSize: int>\n");
		exit(1);
	}

	int memSize = atoi(argv[1]);	// convert argv[1] to an integer
	int* memBlock = (int*)malloc(memSize);	// create memory block of specified size

	memset(memBlock, 0, memSize);	// Wipe the memory block with 0s

	printf("Memory Block Address: %p\n", memBlock);	// output memory block address

	printf("Memory Block Contents: 0x");
	for(int i = 0; i < memSize/sizeof(int); i += 1)
		printf("%08x", *(memBlock + i));	// print out contents  sizeof(int)  bytes at a time
	printf("\n");

	free(memBlock); // frees all memory that memBlock points to
}
