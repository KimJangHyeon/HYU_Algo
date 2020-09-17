#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "LinkedMerge.h"


#define MAXINPUTA 256
#define MAXINPUTE 1000000
#define MININPUT 1

#define MAXVALUE 2147483647
#define MINVALUE -2147483648

#define DIV(a) (((a)%2 == 0) ? (a)/2 : (a+1)/2)

void panic (char err[]) {
	printf("%s\n", err);
	exit(0);

}
//return
//number of linked merge

int divide_count(MergeArray* ma) {
	if (ma->count % 2) {
		ma->check = 1;
		return (ma->count = (ma->count + 1) / 2);
	} else {
		ma->check = 0;
		return (ma->count = ma->count / 2);
	}
}

char* path_finder(int n) {
    char* ret = (char*)malloc(sizeof(char) * 16);
	memset(ret, 0, sizeof(char) * 16);
    const char input[6] = "input";
    char num[4]; 
    const char txt[5] = ".txt";
    
    memcpy(ret, input, 6); 
    sprintf(num, "%d", n); 
    if (n >= 100) {
        strncpy(ret+5, num, 3); 
        strncpy(ret+8, txt, 4); 
    }   
    else if (n >= 10) {
        strncpy(ret+5, num, 2); 
        strncpy(ret+7, txt, 4); 
    }   
    else {
        strncpy(ret+5, num, 1); 
        strncpy(ret+6, txt, 4); 
    }
    return ret;
}



void *
read_file(void * d) {
	Data* data = d;
	int entry;
	while( 0 < fscanf(data->fp, "%d ", &entry)) {
				insert_back(data->lm, entry);
	}	
	fclose(data->fp);

	pthread_exit((void*) 0);
}


int input_file(char* path, MergeArray* ma) {
	int count;
	int n, k;
	FILE *fp;
	int i = 0;
	int old_data, data;
	char num[3];
	pthread_t* threads;
	FILE **fps;
	int status;
	fp = fopen(path, "r");
	fscanf(fp, "%d %d %d", &n, &count, &k);
	fclose(fp);

	if (n < MININPUT || n > MAXINPUTA)
		panic ("first input index should 1 <= N <= 256 !!");

	if (count < MININPUT || count > MAXINPUTE) 
		panic ("second input index should 1 <= M <= 1,000,000 !!");
	if (k < MININPUT || k > n * count)
		panic ("third input index should 1 <= K <= N*M !!");

	ma->count = n;
	ma->merge_arr = (LinkedMerge*)malloc(sizeof(LinkedMerge) * n);
	threads = (pthread_t*)malloc(sizeof(pthread_t)*n);
	fps = (FILE**)malloc(sizeof(FILE*)*n);
	for (int i = 0; i < n; i++) {
		Data * data = (Data*)malloc(sizeof(Data));
		init_linked_merge(&(ma->merge_arr[i]));
		fps[i] = fopen(path_finder(i + 1), "r");
		data->fp = fps[i];
		data->lm = &(ma->merge_arr[i]);
		pthread_create(&threads[i], NULL, &read_file, (void*)data);	
	}
	for (int i = n - 1; i >= 0 ; i-- ) {
		pthread_join(threads[i], (void**)&status);
	}
	return k;
}




void output_file(char* path, LinkedMerge* lm, int k) {
	FILE *fp;
	Entry* rst;
	int k_count = 0;
	fp = fopen(path, "w");
	while ((rst = pop_lm(lm))!=NULL) {
		if ( ( (++k_count) % k ) == 0 ) {
			fprintf(fp, "%d ", rst->data);	
		}
	}

}


int main() {
	int k;
	MergeArray* ma = (MergeArray*)malloc(sizeof(MergeArray));
	k = input_file("input.txt", ma);

	if (ma->count != 1) {
		while (divide_count(ma) != 1) {
				for(int i = 0; i < ma->count; i++) {
				if(i == ma->count - 1 && ma->check == 1) {
					linked_merge_conquer(&(ma->merge_arr[i * 2]), NULL, &(ma->merge_arr[i]));
					break;
				}
				linked_merge_conquer(&(ma->merge_arr[i * 2]), &(ma->merge_arr[i * 2 + 1]), &(ma->merge_arr[i]));
			}

		}
		linked_merge_conquer(&(ma->merge_arr[0]), &(ma->merge_arr[1]), &(ma->merge_arr[0]));
		output_file("output.txt", &(ma->merge_arr[0]), k);
	}
}
