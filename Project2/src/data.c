#include <stdio.h>
#include <stdlib.h>

#include "data.h"

BufferPool* 
input_file(char* path) {
    BufferPool* ret;
    FILE* fp; 
    int count;

    fp = fopen(path, "r");
    fscanf(fp, "%d\n", &count); 
    printf("count: %d\n", count);   
    
    ret = (BufferPool*)malloc(sizeof(BufferPool));
    ret->count = count;
    ret->buffers = (Buffer*)malloc(sizeof(Buffer) * count);
    

    for (int i = 0; i < count; i++) {
        fscanf(fp, "%c %d %d\n", &(ret->buffers[i].order.func), &(ret->buffers[i].order.param1), &(ret->buffers[i].order.param2));
    }   
    fclose(fp);
    for (int i = 0; i < count; i++) {
        printf("arr: %c %d %d\n", ret->buffers[i].order.func, ret->buffers[i].order.param1, ret->buffers[i].order.param2);
    }   
    return ret;
}



