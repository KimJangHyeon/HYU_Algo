#include <stdlib.h>
#include <stdio.h>
#include "types.h"
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
			case 'P':
				ret->buffers[i].output.order = PRINTTREE;
				break;
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
    while(1) {
        o = &(bp->buffers[i].output);
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
			fprintf(fp, "Found (%ld, %ld) on d=%d with h=%d\n", o->record[0].key, o->record[0].data, o->depth, o->height);
			//fprintf(fp, "Found (%ld, %ld) on d=%d with h=%d\n", o->record->key, o->record->data);
			//fprintf(fp, "Found (%ld, %ld)\n", o->record->key, o->record->data);
		}
		//print tree
		else if (o->order == PRINTTREE){
			for (int j = 0; j < o->count; j++) {
				fprintf(fp, "(%ld, %ld)  ", o->record[j].key, o->record[j].data);
			}
			fprintf(fp, "\n");
		}
		//none
		else {
			exit(0);
		}
        i++;
    }
    fclose(fp);
}


