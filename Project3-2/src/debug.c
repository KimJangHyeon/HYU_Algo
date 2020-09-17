#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
void
panic(char* err) {
	printf("panic: %s!!!\n", err);
	exit(0);
}
