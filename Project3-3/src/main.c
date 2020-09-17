#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "params.h"
#include "io.h"
#include "algorithm.h"

Header h;

void
init_header() {
	h.node = NULL;
	h.height = 0;
}

void
algo_operator(Order* order, Output* output) {
	switch(order->func) {
		case 'I':
			insert(&h, order->param1, order->param2, output);
			break;
		case 'F':
			find(&h, order->param1, output);
			if (output->order == FIND) {
				//fprintf(stderr, "enter record(%ld, %ld)\n", output->record[0].key, output->record[0].data);
				//fprintf(stderr, "=====================\n");
			}
			else if (output->order == NOTFOUND) {
				//fprintf(stderr, "not found(%ld)\n", order->param1);
			}
			break;
		case 'P':
			//printTree(&h, output);
			break;
	}
}

int main(int argc, char* argv[]) {
    BufferPool* bp; 
   	Output* output;
	Order* order;
	int i = 0;
   
	init_header();
	
	if (argc != 2) {
        printf("form: ./project2-1 input.txt input-keys.txt\n");
        exit(0);
    }   

    bp = input_file(argv[1]);
	
	while(1) {
        order = &(bp->buffers[i].order);
        output = &(bp->buffers[i].output);
		algo_operator(order, output);	
		if (order->func == 'Q')
            break;
        i++;
    }
	output_file("output.txt", bp);
	//output_memory(bp);
}
