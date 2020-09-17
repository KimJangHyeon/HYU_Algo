#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "io.h"
#include "algorithm.h"
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


Node* make_node(ukey_t key, udata_t value) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->key = key;
	n->value = value;
	n->height = 1;
	n->left = NULL;
	n->right = NULL;
	return n;
}


int height(Node* n) {
	if (n == NULL) 
		return 0;
	else 
		return n->height;
}

//clock??
void
cwrotate(Node* n) {
	Node* lt = n->left;
	//give rt->left to n->right
	n->left = lt->right;
	lt->right = n;
	
	n->height = MAX(height(n->right), height(n->left)) + 1;
	lt->height = MAX(height(lt->right), height(lt->left)) + 1;


}

//unclock??
void
ncwrotate(Node* n) {
	Node* rt = n->right;
	//give rt->left to n->right
	n->right = rt->left;
	rt->left = n;
	
	n->height = MAX(height(n->right), height(n->left)) + 1;
	rt->height = MAX(height(rt->right), height(rt->left)) + 1;

}

int height_checker(Node* n) {
	int h;
	if (n == NULL) 
		return 0;

	//printf("left height: %d\n", height(n->left));
	//printf("right height: %d\n", height(n->right));
	h = height(n->left) - height(n->right);
	//n->height = MAX(height(n->right), height(n->left));
	if (h > 1)
		return 1;
	else if (h < -1)
		return -1;
	else 
		return 0;
}

//do recursive
Node* 
insert(Node* node, ukey_t key, udata_t value, Output* o) {
	int hc;
	Node* subroot;
	o->record = (Record*)malloc(sizeof(Record));
	o->record[0].key = key;
	o->record[0].data = value;
	if (node == NULL) {
		o->count = 1;
		return make_node(key, value);
	}
	
	if (key < node->key)
		node->left = insert(node->left, key, value, o);
	else if (key > node->key)
		node->right = insert(node->right, key, value, o);
	else {
		o->odata = node->value;
		o->count = 0;
		node->value = value;
		return node;	//if null then same key exist
	}
	//set height
	//printf("max: %d\n", MAX(height(node->left), height(node->right)));
	//printf("lh: %d\n", height(node->left));
	//printf("rh: %d\n", height(node->right));
	node->height = MAX(height(node->left), height(node->right)) + 1;
	//printf("nodeh: %d\n", node->height);
	//get balance
	hc = height_checker(node);
	//printf("hc: %d\n", hc);
	//right is bigger
	if (hc == -1) {
		//printf("-1 rotate!!\n");
		if (key > node->right->key) {
			//printf("k > n rotate!!\n");
			subroot = node->right;
			//ncwrotate
			ncwrotate(node);
			return subroot; 
		}
		else if (key < node->right->key) {
			//printf("k < n rotate!!\n");
			//node->right = cw rotate
			subroot = node->right->left;
			cwrotate(node->right);
			node->right = subroot;
			//leftrotate
			subroot = node->right;
			ncwrotate(node);
			return subroot;
		}
	}

	//left is bigger
	else if (hc == 1) {
		//printf("1 rotate!!\n");
		if (key < node->left->key) {
			//printf("k < n rotate!!\n");
			subroot = node->left;
			//ncwrotate
			cwrotate(node);
			return subroot; 
		}
		else if (key > node->left->key) {
			//printf("k > n rotate!!\n");
			//node->right = cw rotate
			subroot = node->left->right;
			ncwrotate(node->left);
			node->left = subroot;
			//leftrotate
			subroot = node->left;
			cwrotate(node);
			return subroot;
		}
		else {
			return node;
		}
	}
	return node;

}

FindInfo* 
find(Node* hp, ukey_t key, Output* o) {
	FindInfo* find_info = (FindInfo*)malloc(sizeof(FindInfo));
	Node* pnode;

	//init find_info
	find_info->isFind = 0;
	find_info->height = 0;
	find_info->depth = 0;

	if (hp == NULL) {
		return find_info;
	}
	pnode = hp;
	while(1) {
		if (key > pnode->key) {
			if (pnode->right)  {
				pnode = pnode->right;
				find_info->depth++;
			} else {
				o->order = NOTFOUND;
				return find_info;
			}
		} else if (key < pnode->key) {
			//printf("key: %ld\npkey: %ld\n", key, pnode->key);
			if (pnode->left) {
				pnode = pnode->left;
				find_info->depth++;
			} else {
				o->order = NOTFOUND;
				return find_info;
			}
		} else {
			find_info->height = pnode->height;
			find_info->isFind = 1;
			o->order = FIND;
			o->record = (Record*)malloc(sizeof(Record));
			o->record[0].key = key;
			o->record[0].data = pnode->value;
			if (pnode->height >= 1) 
				o->height = pnode->height - 1;
			else 
				o->height = pnode->height;
			o->depth = find_info->depth;
			return find_info;
		}
	}	
}

void preOrder(Node* n, int* cur,Output* o) {
	if (n == NULL)
		return;
	//printf("%ld(%d) ", n->key, n->height);
	
	o->record[*cur].key = n->key;
	o->record[(*cur)++].data = n->value;

	preOrder(n->left, cur, o);
	preOrder(n->right, cur, o);
}

void
printTree(Header* h, Output* o) {
	o->count = h->count;
	o->record = (Record*)malloc(sizeof(Record) * o->count);
	preOrder(h->node, &(h->cur), o);
	//printf("\n");
	h->cur = 0;
}
