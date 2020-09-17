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
	Hash* h = (Hash*)malloc(sizeof(Hash));;
	//Output* Temp = (Output*)malloc(sizeof(Output));
	init_hash(h, bp->count);
	while(1) {
		order = &(bp->buffers[i].order);
		output = &(bp->buffers[i].output);
		operate_hash(h, order, output);	
		//printf("\n\n[%d]\n", i);
		//printTable(h, Temp);
		
		if (order->func == 'Q')
			break;
		i++;
	}
	i=0;
	output_file("output.txt", bp);
}
