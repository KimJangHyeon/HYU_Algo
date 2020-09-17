#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "data.h"
#include "hash.h"


ukey_t 
hash_func(usize_t size, ukey_t key, bool isFirst) {
	if (isFirst) {
		return ((key * PRIMEHASH) % size);
	}
	else {
		return (((key * PRIMEHASH) / size) % size);
	}
}

void 
init_hash(Hash* h1, Hash* h2, usize_t size) {
	h1->size = (size * LINEARHASH_C);
	h1->datas = (Entry*)malloc(sizeof(Entry) * h1->size);
	h2->size = (size * LINEARHASH_C);
	h2->datas = (Entry*)malloc(sizeof(Entry) * h2->size);
	for (int i = 0; i < h1->size; i++) {
		h1->datas[i].data = HASH_EMPTY;
		h1->datas[i].key = KEY_EMPTY;
	}
	for (int i = 0; i < h2->size; i++) {
		h2->datas[i].data = HASH_EMPTY;
		h2->datas[i].key = KEY_EMPTY;
	}
}

void
rehash(Hash* h1, Hash* h2, usize_t sz, Entry* entry) {
	usize_t osize = h1->size;
	usize_t nsize = osize * osize * HASHJ;
	Output* temp = (Output*)malloc(sizeof(Output));
	printf("REHASH!!!\n");
	free(h1->datas);
	free(h2->datas);
	
	init_hash(h1, h2, nsize);

	for (int i = 0; i < sz; i++) 
		hash_insert(h1, h2, entry[i].key, entry[i].data, temp);
}

usize_t 
get_entry(Hash* h1, Hash* h2, Entry** entry) {
	usize_t recnt = 0;

	*entry = (Entry*)malloc(sizeof(Entry) * (h1->size + h2->size));
	for (usize_t i = 0; i < h1->size; i++) {
		if (h1->datas[i].key != KEY_EMPTY) {
			(*entry)[recnt].key = h1->datas[i].key;
			(*entry)[recnt].data = h1->datas[i].data;
			recnt++;
		}
		if (h2->datas[i].key != KEY_EMPTY) {
			(*entry)[recnt].key = h1->datas[i].key;
			(*entry)[recnt].data = h1->datas[i].data;
			recnt++;
		}
	}
	return recnt;
}

Entry*
place (Hash* h, Entry* entry, ukey_t index) {
	Entry * ret = (Entry*)malloc(sizeof(Entry));
	if (h->datas[index].key == KEY_EMPTY)
		ret = NULL;
	else {
		ret->key = h->datas[index].key;
		ret->data = h->datas[index].data;
	}
	h->datas[index].key = entry->key;
	h->datas[index].data = entry->data;
	return ret;
}

void
hash_insert(Hash* h1, Hash* h2, ukey_t key, int value, Output* output) {

	ukey_t index;
	ukey_t count = 0;
	Entry* entries;
	usize_t recnt;
	Entry* entry = (Entry*)malloc(sizeof(Entry));
	entry->data = value;
	entry->key = key;
	
	//duplicate check
	index = hash_func(h1->size, key, 1);
	if (h1->datas[index].key == key) {
		h1->datas[index].key = key;
		h1->datas[index].data = value;
		strcpy(output->ret, "Duplicated");
		output->val = OUTPUT_VALNULL;
		return;
	}	
	index = hash_func(h2->size, key, 0);
	if (h2->datas[index].key == key) {
		h2->datas[index].key = key;
		h2->datas[index].data = value;

		strcpy(output->ret, "Duplicated");
		output->val = OUTPUT_VALNULL;
		return;
	}

	while (1) {	
		if (count % 2 == 0) {
			index = hash_func(h1->size, key, 1);
			entry = place (h1, entry, index);
		}
		else {
			index = hash_func(h1->size, key, 0);
			entry = place (h2, entry, index);
		}
		count++;
		if (entry == NULL) {
			strcpy(output->ret, "Inserted");
			output->val = OUTPUT_VALNULL;
			return;
		}
		else 
			key = entry->key;
		
		if (count > h1->size * 2) {
			//Rehash condition!!
			recnt = get_entry(h1, h2, &entries);
			rehash(h1, h2, recnt, entries);
			strcpy(output->ret, "Inserted");
			output->val = OUTPUT_VALNULL;
			return;
		}

	}
}

void
hash_delete(Hash* h1, Hash* h2, ukey_t key, Output* output) {

	ukey_t index;

	//for h1
	index = hash_func(h1->size, key, 1);
	if (h1->datas[index].key == key) {
		h1->datas[index].key = KEY_EMPTY;
		strcpy(output->ret, "Deleted");
		output->val = OUTPUT_VALNULL;
		return;
	}

	index = hash_func(h1->size, key, 0);
	if (h2->datas[index].key == key) {
		h2->datas[index].key = KEY_EMPTY;
		strcpy(output->ret, "Deleted");
		output->val = OUTPUT_VALNULL;
		return;
	}

	strcpy(output->ret, "Not Found");
	output->val = OUTPUT_VALNULL;
	return;
}

void
hash_find(Hash* h1, Hash* h2, ukey_t key, Output* output) {

	ukey_t index;
	
	//for h1
	index = hash_func(h1->size, key, 1);
	if (h1->datas[index].key == key)  {
		output->val = h1->datas[index].data;
		return;
	}

	//for h2
	index = hash_func(h2->size, key, 0);
	if (h2->datas[index].key == key)  {
		output->val = h2->datas[index].data;
		return;
	}

	strcpy(output->ret, "Not Found");
	output->val = OUTPUT_VALNULL;
	return;
}

void
printTable(Hash* h1, Hash* h2, Output* output) {
	printf("H1: \n");
	for (int i = 0; i < h1->size; i++) {
		if (h1->datas[i].key == KEY_EMPTY) {
			printf("[%d]:N ", i);
			continue;
		}
		printf("[%d]:%lld ", i, h1->datas[i].key);
	}
	printf("\n");
	printf("H2: \n");
	for (int i = 0; i < h2->size; i++) {
		if (h2->datas[i].key == KEY_EMPTY) {
			printf("[%d]:N ", i);
			continue;
		}
		printf("[%d]:%lld ", i, h2->datas[i].key);
	}
	printf("\n\n");
	output->val = OUTPUT_IAMQ;
}

void
operate_hash(Hash* h1, Hash* h2, Order* order, Output* output) {
	int key = order->param1;
	int val = order->param2;

	switch(order->func) {
		case 'I':
			hash_insert(h1, h2, key, val, output);
			break;
		case 'F':
			hash_find(h1, h2, key, output);
			break;
		case 'D':
			hash_delete(h1, h2, key, output);
			break;
		case 'Q':
			printTable(h1, h2, output);
			break;
		default:
			break;

	}
}

