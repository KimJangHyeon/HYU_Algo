#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "LinkedMerge.h"

#define MAXINPUT 100000
#define MININPUT 1

#define MAXVALUE 100000

void
linked_merge_conquer(LinkedMerge* t1, LinkedMerge* t2, LinkedMerge* r) {
	if (t2 == NULL) {
		memcpy(r, t1, sizeof(LinkedMerge));
		return;
	}

	if (t1->front->data >= t2->rear->data) {
		//t2 뒤에 t1붙이기
		t2->rear->next = t1->front;
		t2->rear = t1->rear;
		t2->count = t2->count + t1->count;
		memcpy(r, t2, sizeof(LinkedMerge));
		return;
	}
	else if (t1->rear->data <= t2->front->data) {
		//t1 뒤에 t2붙이기
		t1->rear->next = t2->front;
		t1->rear = t2->rear;
		t1->count = t1->count + t2->count;
		memcpy(r, t1, sizeof(LinkedMerge));
		return;
	}

	LinkedMerge* ret = (LinkedMerge*)malloc(sizeof(LinkedMerge));
	init_linked_merge(ret);
	int limit = t1->count + t2->count;
	for (int i = 0; i < limit; i++) {
		if (t1->count == 0) 
			insert_back (ret, pop_lm(t2)->data);	
		else if (t2->count == 0) 
			insert_back (ret, pop_lm(t1)->data);
		else if (t1->front->data > t2->front->data) 
			insert_back(ret, pop_lm(t2)->data);
		else 
			insert_back(ret, pop_lm(t1)->data);
	}
	memcpy(r, ret, sizeof(LinkedMerge));
	free(ret);
}

Entry* 
make_entry(int data) {
    Entry * ret = (Entry*)malloc(sizeof(Entry));
    ret->data = data;
    ret->next = NULL;
    return ret;
}

void 
init_linked_merge(LinkedMerge* lm) {
    lm->count = 0;
    lm->rear = NULL;
    lm->front = NULL;
}

void 
insert_back(LinkedMerge* lm, int data) {
    Entry* input_entry = make_entry(data);
	Entry* temp;
	if (lm->count == 0) {
		lm->rear = input_entry;
		lm->front = input_entry;
	} else {

		temp = lm->rear;
		lm->rear = input_entry;
		temp->next = lm->rear;
	}
	lm->count++;
}

Entry * 
pop_lm(LinkedMerge* lm) {
	Entry * ret;
	if (lm->count == 0) 
		return NULL;
	if (lm->count == 1) {
		ret = lm->front;
		lm->front = NULL;
		lm->rear = NULL;
		ret->next = NULL;
		lm->count--;
		return ret;
	} else {
		ret = lm->front;
		lm->front = lm->front->next;
		ret->next = NULL;
		lm->count--;
		return ret;
	}
}

void 
print_lm(LinkedMerge* lm) {
	if (lm->count == 0) {
		printf("empty lm!!\n");
		return;
	}
	printf("count: %u\n", lm->count);
	printf("front: %d\n", lm->front->data);
	printf("rear: %d\n", lm->rear->data);
	
	Entry* pivot = lm->front;


	while (pivot->next != NULL) {
		printf("%d ", pivot->data);
		pivot = pivot->next;
	}
	printf("%d\n", pivot->data);
}

void 
print_ma(MergeArray* ma) {
	printf("merge arr count: %d\n", ma->count);
	for(int i = 0; i < ma->count; i++) {
		printf("[%d]\n", i);
		print_lm(&(ma->merge_arr[i]));
		printf("\n");
	}
}
