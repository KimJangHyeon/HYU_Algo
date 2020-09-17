#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXINPUT 100000
#define MININPUT 1

int gf = 0;

void swap (int* target1, int* target2) {
	int temp = *target1;
	*target1 = *target2;
	*target2 = temp;
}

void panic (char err[]) {
	printf("%s\n", err);
	exit(0);
}

int
partition(int (*arr)[], int start, int end) {
	int pivot;
	int p_index;
	int l = 0, r = 0;
	srand(time(NULL));

	gf++;

	//init
	p_index = start + (rand() % (end - start + 1));
	pivot = (*arr)[p_index];
	swap(&((*arr)[start]), &((*arr)[p_index]));

	for (int i = start + 1; i <= end; i ++) {
		if ((*arr)[i] > pivot) {
			r++;
		} else {
			l++;
			swap(&((*arr)[start + l]), &((*arr)[i]));
		}
	}
	swap(&((*arr)[start]), &((*arr)[start + l]));
	return start + l;
}

void
quick_sort(int (*arr)[], int start, int end) { 
	int pivot;
	if (start < end) {
		pivot = partition ( arr, start, end);
		quick_sort( arr, start, pivot - 1 );
		quick_sort( arr, pivot + 1, end );
	}
}

int input_file(char* path, int (*arr)[]) {
	int count = 0;
	FILE *fp;
	int i = 0;

	fp = fopen(path, "r");
	while( 0 < fscanf(fp, "%d ", &(*arr)[i])) {
		printf("%d\n", (*arr)[i]);
		count++;
		i++;
	}
	fclose(fp);
	if (count < MININPUT || count > MAXINPUT) 
		panic("input !!!");
	return count;
}




void output_file(char* path, int arr[], int count) {
	FILE *fp;

	fp = fopen(path, "w");
	
	for (int i = 0; i < count; i++) { 
		fprintf(fp, "%d ", arr[i]);	
	}
}



int main() {
	int arr[MAXINPUT];
	int count;
	count = input_file("input.txt", &arr);

	quick_sort(&arr, 0, count - 1);
	

	printf("entry number: %d\n", count);
	printf("number of function called: %d\n", gf);
	for (int i = 0; i < count; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	output_file("output.txt", arr, count);
}
