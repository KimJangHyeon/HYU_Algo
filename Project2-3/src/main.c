#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "data.h"
#include "hash.h"

int 
main(int argc, char* argv[]) {
	int i =0;
	BufferPool* bp;

	if (argc != 2) {
		printf("form: ./project2-1 input.txt\n");
		exit(0);
	}

	bp = input_file(argv[1]);
	
	Order* order;
	Output* output;
	Output* test = (Output*)malloc(sizeof(Output));
	Hash* h1 = (Hash*)malloc(sizeof(Hash));
	Hash* h2 = (Hash*)malloc(sizeof(Hash));
	init_hash(h1, h2, bp->count);
	while(1) {
		order = &(bp->buffers[i].order);
		output = &(bp->buffers[i].output);
		operate_hash(h1, h2, order, output);		
		//printTable(h1, h2, test);
		printf("\n");
		if (order->func == 'Q')
			break;
		i++;
	}
	i=0;
	output_file("output.txt", bp);
}
