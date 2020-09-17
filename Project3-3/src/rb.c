#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "types.h"
#include "params.h"
#include "io.h"
#include "algorithm.h"
#include "debug.h"
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int myrandom() {
	int count = 0;
	int coin = 1;
	srand(time(NULL));

	while(coin) {
		if (count == INTMAX) {
			printf("max height\n");
			break;
		}
		coin = rand()%2;
		count++;
	}
	return count;
}

Node* 
make_node(ukey_t key, udata_t value) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->key = key;
	n->data = value;
	n->next = NULL;
	n->down = NULL;
	//n->up = NULL;
	return n;
}

Node** 
prenode_path(Header* root, ukey_t key, int height) {
	Node** ret; 
	Node* n = root->node;
	Node* pn;
	int tier_index = height - 1;
	if (height == 0) {
		return NULL;
	}else {
		ret = (Node**)malloc(sizeof(Node*)* height);
	}
	for (int i = tier_index; i >= 0; i--) {
		while(1) {
			//is end of the link
			if (n == NULL) {
				ret[i] = pn;
				break;
			}
			if (n->key > key) {
				ret[i] = pn;
				break;
			} 

			pn = n;
			n = n->next;
		}

		//if (n->down != NULL) {
		n = pn->down;
		//} else {
			
		//}
	}
	return ret;
}

Node*
make_tier(Header* root, Node* target_node, ukey_t key, udata_t data) {
	Node* new_root_node = make_node(MINKEY, 0);
	Node* tier_target_node = make_node(key, data);

	new_root_node->next = tier_target_node;
	new_root_node->down = root->node;
	tier_target_node->down = target_node;

	root->node = new_root_node;
	return tier_target_node;
}

void
find(Header* root, ukey_t key, Output* o) {
	Node* n = root->node;
	Node* pn;
	int tier_index = root->height - 1;

	o->record = (Record*)malloc(sizeof(Record));
	if (root->height == 0) {
		//tree is empty
		o->order = NOTFOUND;
		//free(o->record);
		return;
	}
	//fprintf(stderr, "key: %ld\n", key);
	for (int i = tier_index + 1; i >= 0; i--) {
		while(1) {
			//is end of the link
			if (n == NULL) {
				//fprintf(stderr, "enter 1\n");
				n = pn;
				break;
			}
			if (n->key > key) {
				//fprintf(stderr, "%ld", pn->key);
				if (pn->next) {
				//	fprintf(stderr, "(%ld) ", pn->next->key);
				}
				else {
				//	fprintf(stderr, "(null) ");
				}
				break;
			} else if (n->key == key) {
				//found key
				o->order = FIND;
				o->record[0].key = n->key;
				o->record[0].data= n->data;
				//printf("found(%ld, %ld)\n", o->record[0].key, o->record[0].data);
				return;
			} else {
				pn = n;
				n = n->next;
			}
		}

		n = pn->down;
	}
	if (n == NULL) {
		o->order = NOTFOUND;
		//free(o->record);
		//printf("not found(%ld)\n", key);
	} else {
		while(1) {
			if (!n) {
				o->order = NOTFOUND;
				break;
			}
			if (n->key < key) {
				n = n->next;
			}
			else if (n->key == key) {
				o->order = FIND;
				o->record[0].key = n->key;
				o->record[0].data = n->data;
			} else {
				o->order = NOTFOUND;
				//free(o->record);
			}
		}
	}
	fprintf(stderr, "\n");
	return;
}

void
insert(Header* root, ukey_t key, udata_t data, Output* o) {
	int height, rheight, theight;	
	Node** prenode_arr;
	Node* temp;
	Node* n;
	Node* pttn = NULL;
	
	o->order = INSERT;
	o->record = (Record*)malloc(sizeof(Record));
	o->record[0].key = key;
	o->record[0].data = data;
	
	//find index
	//insert index at leaf
	height = myrandom();
	rheight = root->height;

	prenode_arr = prenode_path(root, key, rheight);
	//update
	if (prenode_arr && (prenode_arr[0]->key == key)) {
		o->odata = prenode_arr[0]->data;
		o->count = 0;
	}
	//current tier inserting
	else if(prenode_arr) {
		if (height > rheight)
			theight = rheight;
		else 
			theight = height;
		for (int i = 0; i < theight; i++) {
			n = make_node(key, data);
			temp = prenode_arr[i]->next;
			n->next = temp;
			prenode_arr[i]->next = n;
	
			if (pttn != NULL) {
				n->down = pttn;	
			}
			pttn = n;
		}
		
		for (int i = height - rheight; i > 0; i--) {
			pttn = make_tier(root, pttn, key, data);
		}
	
		o->count = 1;
	}
	else {
		make_tier(root, NULL, key, data);
		o->count = 1;
	}
	//if( o->count == 0) {
		//printf("update(%ld): %ld -> %ld \n", o->record[0].key, o->odata, o->record[0].data);
	//}
	//else {
		//printf("insert(%ld, %ld)\n", o->record[0].key, o->record[0].data);
	//}
	if (rheight < height) {
		root->height = height;
	//	fprintf(stderr, "height: %d\n", root->height);
	}
}
/*
void 
find(Node* root, ukey_t key, Output* o) {
	Node* fnode;
	int depth;

	fnode = find_leaf(root, key, &depth);
	if (key != fnode->key) {
		o->order = NOTFOUND;	
	}
	else {
		//o->order = FIND;
		o->record = (Record*)malloc(sizeof(Record));
		o->record[0].key = fnode->key;
		o->record[0].data= fnode->data;
		o->color = fnode->color;
		o->depth = depth;
	}
	if(o->order == NOTFOUND) {
		printf("not found(%ld)!!\n", key);
	}
	else {
		if (o->order == RD)
			printf("found(%ld, %ld) on d: %d clr: RED\n", o->record[0].key, o->record[0].data, depth);
		else 
			printf("found(%ld, %ld) on d: %d clr: BLACK\n", o->record[0].key, o->record[0].data, depth);

	}
}

*/
