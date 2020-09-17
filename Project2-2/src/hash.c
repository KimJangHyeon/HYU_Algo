#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "data.h"
#include "hash.h"

usize_t sq(usize_t n) {
	usize_t sum = n;
	sum *= n;
	return sum;
}


int isPrime(usize_t number) {
	int lcnt = 0;
	for (lcnt = 2; lcnt < number; lcnt++) 
	{
		if( (number % lcnt) == 0)
		{
			return 0;
		}
	}
	return 1;
}

usize_t getUpPrime(usize_t number) {
	number += 2;
	while(1) {
		if (isPrime(number))
			return number;
		else
			number++;
	}
}

usize_t getDownPrime(usize_t number) {
	number += 2;
	while(1) {
		if (isPrime(number))
			return number;
		else
			number--;
	}
}

ukey_t 
hash_func(usize_t size, ukey_t key, usize_t uprime, usize_t dprime, bool isFirst) {
	if (isFirst)
		return ((key * uprime + dprime) % size);
	else 
		return ((key * dprime + uprime) % size);
}

void 
init_hash(Hash* h, usize_t size, usize_t sz, ukey_t* arr) {
	usize_t* temp = (usize_t*)malloc(sizeof(usize_t) * (size * LINEARHASH_C));
	usize_t index;
	
	h->size = (size * LINEARHASH_C);
	h->uprime =  getUpPrime(h->size);
	h->dprime = getDownPrime(h->size);

	h->ht = (HashS*)malloc(sizeof(HashS) * h->size);
	memset(temp, 0, sizeof(usize_t) * h->size);
	for (int i = 0; i < sz; i++) {
		index = hash_func(h->size, arr[i], h->uprime, h->dprime, 1);
		temp[index]++;
	}

	for (int i = 0; i < h->size; i++) {
		h->ht[i].size = sq(temp[i]) * SHASH_C;
		h->ht[i].uprime = getUpPrime(h->ht[i].size);
		h->ht[i].dprime = getDownPrime(h->ht[i].size);
		if (h->ht[i].size == 0) {
			h->ht[i].size = 1 * SHASH_C;
			h->ht[i].datas = (Entry*)malloc(sizeof(Entry) * h->ht[i].size);
			for (int j = 0; j < h->ht[i].size; j++) {
				h->ht[i].datas[j].key = KEY_EMPTY;
				h->ht[i].datas[j].data = HASH_EMPTY;
			}
		}
		else {
			h->ht[i].datas = (Entry*)malloc(sizeof(Entry) * h->ht[i].size);
			for (int j = 0; j < h->ht[i].size; j++) {
				h->ht[i].datas[j].key = KEY_EMPTY;
				h->ht[i].datas[j].data = HASH_EMPTY;
			}
		}
	}
	free(temp);
}


void
rehash (Hash* h, ukey_t index, Entry* datas, usize_t sz) {
	ukey_t index2;
	Entry* temp = h->ht[index].datas;
	ukey_t key;
	int data;
	bool isSafe = 1;
	printf("REHASH!!!\n");
	usize_t osize = h->ht[index].size;
	usize_t nsize = sq(osize) * 7;

	h->ht[index].datas = (Entry*)malloc(sizeof(Entry) * nsize);
	h->ht[index].size = nsize;
	h->ht[index].uprime = getUpPrime(h->ht[index].size);
	h->ht[index].dprime = getDownPrime(h->ht[index].size);

	for (int j = 0; j < h->ht[index].size; j++) {
		h->ht[index].datas[j].key = KEY_EMPTY;
		h->ht[index].datas[j].data = HASH_EMPTY;
	}

	for (int i = 0; i < sz; i++) {
		key = datas[i].key;
		data = datas[i].data;	
		index2 = hash_func(nsize, key, h->ht[index].uprime, h->ht[index].dprime, 0);
		if (h->ht[index].datas[index2].key == KEY_EMPTY) {
			h->ht[index].datas[index2].key = key;
			h->ht[index].datas[index2].data = data;
		}
		else {
			isSafe = 0;
			break;
		}
		if (i == sz - 1) 
			free(temp);
	}
	
	if (isSafe == 0) {
		rehash(h, index, datas, sz);
		free(temp);
	}
}

void
hash_insert(Hash* h, ukey_t key, int value, Output* output) {
	usize_t recnt = 0;
	Entry* temp;

	usize_t index = hash_func(h->size, key, h->uprime, h->dprime, 1);
	usize_t index2 = hash_func(h->ht[index].size, key, h->ht[index].uprime, h->ht[index].dprime, 0);

	if (h->ht[index].datas[index2].key == KEY_EMPTY) {
		h->ht[index].datas[index2].key = key;	
		h->ht[index].datas[index2].data = value;

		strcpy(output->ret, "Inserted");
		output->val = OUTPUT_VALNULL;
	} else {
		if (h->ht[index].datas[index2].key == key) {	
			h->ht[index].datas[index2].key = key;	
			h->ht[index].datas[index2].data = value;
		
			strcpy(output->ret, "Duplicated");
			output->val = OUTPUT_VALNULL;	
		}
		else {
			temp = (Entry*)malloc(sizeof(Entry) * (h->ht[index].size + 1));
			for (int i = 0; i < h->ht[index].size; i++) {
				if (h->ht[index].datas[i].key != KEY_EMPTY) {
					temp[recnt].key = h->ht[index].datas[i].key;
					temp[recnt].data = h->ht[index].datas[i].data;
					recnt++;
				}
			}
			rehash (h, index, temp, recnt);
			free(temp);
		}
	}
}

void
hash_delete(Hash* h, ukey_t key, Output* output) {
	usize_t index = hash_func(h->size, key, h->uprime, h->dprime, 1);
	usize_t index2 = hash_func(h->ht[index].size, key, h->ht[index].uprime, h->ht[index].dprime, 0);


	if (h->ht[index].datas[index2].key == KEY_EMPTY) {
		strcpy(output->ret, "Not Found");
		output->val = OUTPUT_VALNULL;
		return;
	}
	h->ht[index].datas[index2].key = KEY_EMPTY;
	strcpy(output->ret, "Deleted");
	output->val = OUTPUT_VALNULL;
}

void
hash_find(Hash* h, ukey_t key, Output* output) {
	usize_t index = hash_func(h->size, key, h->uprime, h->dprime, 1);
	usize_t index2 = hash_func(h->ht[index].size, key, h->ht[index].uprime, h->ht[index].dprime, 0);


	if (h->ht[index].datas[index2].key == KEY_EMPTY) {
		strcpy(output->ret, "Not Found");
		output->val = OUTPUT_VALNULL;
		return;
	}
	output->val = h->ht[index].datas[index2].data;
}

void
printTable(Hash* h, Output* output) {
	for (int i = 0; i < h->size; i++) {
		printf("[%d, %lld]: ", i, h->ht[i].size);
		for (int j = 0; j < h->ht[i].size; j++) {
			if (h->ht[i].datas[j].key == KEY_EMPTY)
				printf("N ");
			else
				printf("%lld ", h->ht[i].datas[j].key);
		}
		printf("\n");
	}
	printf("\n\n");
	output->val = OUTPUT_IAMQ;
}

void
operate_hash(Hash* h, Order* order, Output* output) {
	int key = order->param1;
	int val = order->param2;

	switch(order->func) {
		case 'I':
			hash_insert(h, key, val, output);
			break;
		case 'F':
			hash_find(h, key, output);
			break;
		case 'D':
			hash_delete(h, key, output);
			break;
		case 'Q':
			printTable(h, output);
			break;
		default:
			break;

	}
}

