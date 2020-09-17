#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "data.h"

int 
main() {
	BufferPool* bp;
	bp = input_file("input.txt");

	printf("count: %d\n", bp->count);
}
