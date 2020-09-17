#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "data.h"
#include "hash.h"

void
push_hash(Hash* h, usize_t index, Entry* entry) {
	h->datas[index].key = entry->key;
	h->datas[index].data = entry->data;
	for (int i = 0; i < NSTONE; i++) {
	}
}

Entry *
pop_hash(Hash* h, usize_t index) {
	Entry* ret = (Entry*)malloc(sizeof(Entry));
	ret->key = h->datas[index].key;
	ret->data = h->datas[index].data;

	h->datas[index].key = KEY_EMPTY;
	for (int i = 0; i < NSTONE; i++) {
		h->datas[index].checker[i] = 0;
	}
	for (int i = 0; i < NSTONE; i++) {
		if (h->datas[(h->size + index - i - 1) % h->size].checker[i] == 1)
			h->datas[(h->size + index - i - 1) % h->size].checker[i] = 0;
	}
	return ret;
}

ukey_t 
strider(Hash* h, ukey_t index, int limit) {
	ukey_t pindex = (h->size + index - limit) % h->size;
	for (int i = 0; i < NSTONE; i++) {
		if ( h->datas[pindex].checker[i] == 0)
			return ((pindex + i) % h->size);
	}
	return -1;
}

void
comeOnYoh11(Hash* h, ukey_t dest, ukey_t cur) {

	if (dest > cur) 
		cur = h->size + cur;
	
}

ukey_t 
find_empty(Hash* h, ukey_t index) {
	for (int i = 0; i < h->size; i++) {
		if (h->datas[(index + i) % h->size].key == KEY_EMPTY )
			return ((index + i) % h->size);
	}
	return -1;
}

void
bit_checker(Hash* h, ukey_t key, ukey_t index, bool in) {
	ukey_t k;
	if (key == -1)
		return;
	k = key % (h->size);
	
	for (int i = 0; i < NSTONE; i++) {
		if (k == (h->size + index - i) % (h->size)) {
			if (in)
				h->datas[(h->size + index - i) % (h->size)].checker[i] = 1;
			else
				h->datas[(h->size + index - i) % (h->size)].checker[i] = 0;
		}
	}
}

void
first_insert(Hash* h, ukey_t key, int value, Output* output, ukey_t* dest, ukey_t* cur) {
	ukey_t store_index = -1;
	ukey_t index;
	Entry entry;
	entry.key = key;
	entry.data = value;
	
	*dest = key % (h->size);

	for (int i = 0; i < h->size; i++) {
		index = (key + i) % (h->size);
		//store
		if ((store_index == -1) && (h->datas[index].key == KEY_EMPTY)) 
			store_index = index;

		//duplicate
		if (h->datas[index].key == key) {
			push_hash(h, index, &entry);
			//h->datas[index].key = key;
			//h->datas[index].data = value;
			*cur = index;
						return;
		}
	}
	if (store_index != -1) {
		//linear probe
		push_hash(h, store_index, &entry);
		*cur = store_index;
		//h->datas[store_index].key = key;
		//h->datas[store_index].data = value;
		strcpy(output->ret, "Inserted");
		output->val = OUTPUT_VALNULL;
		return;
	}
	else {
		//hash full!!
		printf("HASH FULL EXIT!!!\n");
		exit(0);
	}


}


void 
init_hash(Hash* h, usize_t size) {
	h->size = (size * LINEARHASH_C);
	h->datas = (Entry*)malloc(sizeof(Entry) * h->size);
	for (usize_t i = 0; i < h->size; i++) {
		h->datas[i].data = HASH_EMPTY;
		h->datas[i].key = KEY_EMPTY;
		for (int j = 0; j < NSTONE; j++) {
			h->datas[i].checker[j] = 0;
		}
	}
}


void
hash_insert(Hash* h, ukey_t key, int value, Output* output) {
	ukey_t index = key % (h->size);
	ukey_t eindex, oindex;
	ukey_t store_index = -1;
	Entry ttry;
	int store_bit = -1;
	for (int i = 0; i < NSTONE; i++) {
		if (h->datas[index].checker[i] == 1) {
			if (key == h->datas[i + index].key) {
				//duplicate
				h->datas[index].key = key;
				h->datas[index].data = value;
				strcpy(output->ret, "Duplicated");
				output->val = OUTPUT_VALNULL;

				return;
			}
		}
		else {
			if ((store_index == -1) && (h->datas[index + i].key == KEY_EMPTY)) {
				store_index = index + i;
				store_bit = i;
			}
		}
	}
	if (store_index != -1) {
		//insert
		h->datas[store_index].key = key;
		h->datas[store_index].data = value;
		h->datas[index].checker[store_bit] = 1;
		
		strcpy(output->ret, "Inserted");
		output->val = OUTPUT_VALNULL;

		return;
	}
	//=============================================


	eindex = find_empty(h, index);

	while (((h->size + eindex - index) % h->size) >= NSTONE) {
		oindex = eindex;
		eindex = strider(h, eindex, NSTONE);
		if (eindex == -1) {
		
			strcpy(output->ret, "Inserted");
			output->val = OUTPUT_VALNULL;

			return;
		}
		bit_checker(h, h->datas[oindex].key, oindex, 0);
		bit_checker(h, h->datas[eindex].key, eindex, 0);
		bit_checker(h, h->datas[oindex].key, eindex, 1);
		bit_checker(h, h->datas[eindex].key, oindex, 1);
		ttry.key = h->datas[oindex].key;
		ttry.data = h->datas[oindex].data;
		
		h->datas[oindex].key = h->datas[eindex].key;
		h->datas[oindex].data = h->datas[eindex].data;

		h->datas[eindex].key = ttry.key;
		h->datas[eindex].data = ttry.data;

	//	bit_checker(h, h->datas[oindex].key, oindex, 1);
	//	bit_checker(h, h->datas[eindex].key, eindex, 1);
	}
	
	for (int i = 0; i < NSTONE; i++) {
		if (h->datas[index].checker[i] == 0) {
			bit_checker(h, h->datas[i + index].key, i + index, 0);
			h->datas[eindex].key = h->datas[i + index].key;
			h->datas[eindex].data = h->datas[i + index].data;
			bit_checker(h, h->datas[eindex].key, eindex, 1);

			h->datas[i + index].key = key;
			h->datas[index + i].data = value;
			h->datas[index].checker[i] = 1;
			
			strcpy(output->ret, "Inserted");
			output->val = OUTPUT_VALNULL;

			return;
		}
	}
	
	//printf("\n\n");
	//printTable(h, output);
}

void
hash_delete(Hash* h, ukey_t key, Output* output) {
	int index = key % (h->size);

	for (int i = 0; i < NSTONE; i++) {
		if (h->datas[index].checker[i] == 1) {
			if (h->datas[i + index].key == key) {
				h->datas[i + index].key = KEY_EMPTY;
				h->datas[index].checker[i] = 0;
				strcpy(output->ret, "Deleted");
				output->val = OUTPUT_VALNULL;
				return;
			}
		}
	}

	for (int i = 0; i < h->size; i++) {
		if (h->datas[(index + i) % (h->size)].key == key) {
			h->datas[(index + i) % (h->size)].key = KEY_EMPTY;
			strcpy(output->ret, "Deleted");
			output->val = OUTPUT_VALNULL;
			return;
		}
	}

	strcpy(output->ret, "Not Found");
	output->val = OUTPUT_VALNULL;
}


void
hash_find(Hash* h, ukey_t key, Output* output) {
	ukey_t index = key % (h->size);

	for (int i = 0; i < NSTONE; i++) {
		if (h->datas[index].checker[i] == 1) {
			if (h->datas[i + index].key == key) {
				output->val = h->datas[(index + i) % (h->size)].data;
				return;
			}
		}
	}

	for (int i = 0; i < h->size; i++) {
		if (h->datas[(index + i) % (h->size)].key == key) {
			output->val = h->datas[(index + i) % (h->size)].data;
			return;
		}
	}

	strcpy(output->ret, "Not Found");
	output->val = OUTPUT_VALNULL;
}

void
printTable(Hash* h, Output* output) {
	for (int i = 0; i < h->size; i++) {
		if (h->datas[i].key == KEY_EMPTY) {
			printf("data[%d]: NULL ", i);
			for (int j = 0; j < NSTONE; j++) {
				printf("%d ", h->datas[i].checker[j]);
			}
			printf("\n");

			continue;
		}
		printf("data[%d]: (k:%lld, i:%lld, d:%d) ", i,  h->datas[i].key, (h->datas[i].key) % (h->size),h->datas[i].data);
		for (int j = 0; j < NSTONE; j++) {
			printf("%d ", h->datas[i].checker[j]);
		}
		printf("\n");
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

