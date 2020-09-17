#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "io.h"
#include "algorithm.h"

Header h;

void
init_header() {
	h.node = NULL;
	h.count = 0;
	h.cur = 0;
}

void
tree_operator(Order* order, Output* output) {
	switch(order->func) {
		case 'I':
			//printf("=-----------%ld-----------\n", order->param1);
			h.node = insert(h.node, order->param1, order->param2, output);
			if (output->count == 1) 
				h.count++;
			break;
		case 'F':
			find(h.node, order->param1, output);
			break;
		case 'P':
			printTree(&h, output);
	}
}
int main(int argc, char* argv[]) {
    BufferPool* bp; 
	FindInfo* fi;
   	Output* output;
	Order* order;

    init_header();
	
	if (argc != 2) {
        printf("form: ./project2-1 input.txt input-keys.txt\n");
        exit(0);
    }   

    bp = input_file(argv[1]);
	int i = 0;
	while(1) {
        order = &(bp->buffers[i].order);
        output = &(bp->buffers[i].output);
		tree_operator(order, output);	
		if (order->func == 'Q')
            break;
        i++;
    }
	output_file("output.txt", bp);

}
