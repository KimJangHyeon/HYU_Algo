#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "data.h"
#include "hash.h"

int 
main(int argc, char* argv[]) {
	int i =0;
	ukey_t cnt;
	BufferPool* bp;
	ukey_t* input_keys = NULL;

	if (argc != 3) {
		printf("form: ./project2-1 input.txt input-keys.txt\n");
		exit(0);
	}

	bp = input_file(argv[1]);
	cnt = input_key_file(argv[2], &input_keys);
	Order* order;
	Output* output;
	Hash* h = (Hash*)malloc(sizeof(Hash));;
	//add input-key file
	
//	for (int i = 0; i < cnt; i++) {
//		printf("%lld ", input_keys[i]);
//	}
//	printf("\n");
	init_hash(h, bp->count, cnt, input_keys);
	while(1) {
		order = &(bp->buffers[i].order);
		output = &(bp->buffers[i].output);
		operate_hash(h, order, output);		
		if (order->func == 'Q')
			break;
		i++;
	}
	i=0;
	output_file("output.txt", bp);
}
