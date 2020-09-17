#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "data.h"
#include "hash.h"

void 
init_hash(Hash* h, usize_t size) {
	h->size = (size * LINEARHASH_C);
	h->datas = (Entry*)malloc(sizeof(Entry) * h->size);
	for (usize_t i = 0; i < h->size; i++) {
		h->datas[i].data = HASH_EMPTY;
		h->datas[i].key = KEY_EMPTY;
	}
}

void
hash_insert(Hash* h, ukey_t key, int value, Output* output) {
	ukey_t store_index = -1;
	ukey_t index;
	
	for (int i = 0; i < h->size; i++) {
		index = (key + i) % (h->size);
		//store
		if ((store_index == -1) && (h->datas[index].key == KEY_EMPTY)) 
			store_index = index;

		//duplicate
		if (h->datas[index].key == key) {
			h->datas[index].key = key;
			h->datas[index].data = value;
		
			strcpy(output->ret, "Duplicated");
			output->val = OUTPUT_VALNULL;
			return;
		}
	}
	if (store_index != -1) {
		//linear probe
		h->datas[store_index].key = key;
		h->datas[store_index].data = value;
		strcpy(output->ret, "Inserted");
		output->val = OUTPUT_VALNULL;
		return;
	}
	else {
		//hash full!!
		printf("HASH FULL EXIT!!!\n");
		printTable(h, output);
		exit(0);
	}

}

void
hash_delete(Hash* h, ukey_t key, Output* output) {
	int index;

	for (int i = 0; i < h->size; i++) {
		index = (key + i) % (h->size);
		if (h->datas[index].key == key) {
			h->datas[index].key = KEY_EMPTY;
		
			strcpy(output->ret, "Deleted");
			output->val = OUTPUT_VALNULL;
			return;
		}
	}

	strcpy(output->ret, "Not Found");
	output->val = OUTPUT_VALNULL;

}


ukey_t
hash_find(Hash* h, ukey_t key, Output* output) {
	ukey_t index;
	for (ukey_t i = 0; i < h->size; i++) {
		index = (key + i) % (h->size);
		if (h->datas[index].key == key) {
			output->val = h->datas[index].data;
			return index;
		}
	}
	strcpy(output->ret, "Not Found");
	output->val = OUTPUT_VALNULL;
	return -1;
}

void
printTable(Hash* h, Output* output) {
	for (int i = 0; i < h->size; i++) {
		if (h->datas[i].key == KEY_EMPTY) {
			printf("data[%d]: NULL!!\n", i);
			continue;
		}
		printf("data[%d]: (k:%lld, d:%d)\n", i, h->datas[i].key, h->datas[i].data);
	}

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

