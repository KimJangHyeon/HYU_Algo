#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "params.h"
#include "io.h"
#include "algorithm.h"	//AVL

BufferPool*
input_file(char* path) {
    BufferPool* ret;
    FILE* fp; 

    fp = fopen(path, "r");
        
    ret = (BufferPool*)malloc(sizeof(BufferPool));
    int i = 0;
    while(1) {
        fscanf(fp, "%c %ld %ld\n", &(ret->buffers[i].order.func), &(ret->buffers[i].order.param1), &(ret->buffers[i].order.param2));
		switch(ret->buffers[i].order.func) {
			case 'I':
				ret->buffers[i].output.order = INSERT;
				break;
			case 'F':
				ret->buffers[i].output.order = FIND;
				break;
			//case 'P':
			//	ret->buffers[i].output.order = PRINTTREE;
			//	break;
			default:
				ret->buffers[i].output.order = NONE;
				break;
		}
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
	//fprintf(stderr, "output\n");
    while(1) {
        o = &(bp->buffers[i].output);
		if (o == NULL)
			fprintf(stderr, "bp->buffers[i].output is NULL\n");
		//not found
        if (o->order == NOTFOUND) {
            fprintf(fp, "Not Found\n");
        }
		//insert
		else if (o->order == INSERT) {
			//update
			if (o->count == 0) {
				fprintf(fp, "Found(%ld, %ld) update v=%ld\n",o->record[0].key, o->odata, o->record[0].data);
			} else {
				fprintf(fp, "Inserted(%ld, %ld)\n", o->record[0].key, o->record[0].data);
			}
		}
		//find
		else if (o->order == FIND) {
				fprintf(fp, "Found (%ld, %ld)\n", o->record[0].key, o->record[0].data);
		}
		//none
		else {

			exit(0);
		}
        i++;
    }
    fclose(fp);
}

void
output_memory(BufferPool* bp) {
	Output* o;
	int i = 0;
	fprintf(stderr, "output memo\n");
	while(1) {
        o = &(bp->buffers[i].output);
		if (o == NULL)	
			fprintf(stderr, "bp->buffers[i].output is NULL\n");
		
		//fprintf(stderr, "==========1 o exist=============\n");
		//not found
        if (o->order == NOTFOUND) {
			//fprintf(stderr, "==========2 not found=============\n");
            fprintf(stderr, "Not Found\n");
        }
		//insert
		else if (o->order == INSERT) {

			//fprintf(stderr, "==========2 insert=============\n");
			//update
			if (o->count == 0) {
				fprintf(stderr, "Found(%ld, %ld) update v=%ld\n",o->record[0].key, o->odata, o->record[0].data);
			} else {
				fprintf(stderr, "Inserted(%ld, %ld)\n", o->record[0].key, o->record[0].data);
			}
		}
		//find
		else if (o->order == FIND) {
			//fprintf(stderr, "==========2 find=============\n");
			fprintf(stderr, "Found (%ld, %ld)\n", o->record[0].key, o->record[0].data);
		}
		//none
		else {

			exit(0);
		}
        i++;
    }

}
