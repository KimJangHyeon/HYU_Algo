#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "data.h"
#include "hash.h"
BufferPool* 
input_file(char* path) {
    BufferPool* ret;
    FILE* fp; 
    int count;

    fp = fopen(path, "r");
    fscanf(fp, "%d\n", &count); 
    
    ret = (BufferPool*)malloc(sizeof(BufferPool));
    ret->count = count;
	int i = 0;
    while(1) {
        fscanf(fp, "%c %d %d\n", &(ret->buffers[i].order.func), &(ret->buffers[i].order.param1), &(ret->buffers[i].order.param2));

		if (ret->buffers[i].order.func == 'Q')
			break;
		i++;
    }   
    fclose(fp);
    return ret;
}

void 
output_file(char* path, BufferPool* bp) {
	Output *o;
	FILE* fp = fopen(path, "w");
	int i = 0;
	while(1) {
		o = &(bp->buffers[i].output);
		if (o->val == OUTPUT_VALNULL) {
			fprintf(fp, "%s\n", o->ret);
		}
		else if (o->val == OUTPUT_IAMQ) {
			break;
		}
		else {
			fprintf(fp, "%d\n", o->val);
		}
		i++;
	}
	fclose(fp);
}

