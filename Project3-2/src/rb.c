#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "params.h"
#include "io.h"
#include "algorithm.h"
#include "debug.h"
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


Node* 
make_node(ukey_t key, udata_t value) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->key = key;
	n->data = value;
	n->color = RD;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	return n;
}

void
recolor(Node* n) {
	if (n == NULL)
		panic("recolor target node is NULL");
	if (n->left == NULL) 
		panic("recolor target node left is NULL");
	if (n->right == NULL)
		panic("recolor target node right is NULL");

	n->color = RD;
	n->left->color = BK;
	n->right->color = BK;
}


//tree empty ==> null
//
Node* 
find_leaf(Node* root, ukey_t key, int* d) {
	Node* n;
	
	*d = 0;
	if (root == NULL) {
		return NULL;
	}

	n = root;

	while (1) {
		if (n->key == key)
			return n;
		else if (n->left && n->key > key) {
			n = n->left;
		} else if (n->right && n->key < key) {
			n = n->right;
		}
		else {
			return n;
		}
		(*d)++;
	}
	
}


void
node_swap(Node* p, Node* c, char isLeft) {
	
}

//clock??
// check this root works
void
cwrotation(Node** r, Node* n) {
	Node* ln = n->left;
	Node* root = (*r);

	//give rt->left to n->right
	n->left = ln->right;

	if (n->left != NULL)
		n->left->parent = n;
	
	ln->parent = n->parent;

	if (ln->parent == NULL)
		root = ln;
	else if (ln->parent->left == n)
		n->parent->left = ln;
	else if (ln->parent->right == n)
		n->parent->right = ln;
	else 
		panic("cwrotate ln parent has no value n");

	ln->right = n;
	n->parent = ln;
	(*r) = root;
}

//unclock??
void
ucwrotation(Node** r, Node* n) {
	Node* rn = n->right;
	Node* root = (*r);

	//give rt->left to n->right
	n->right = rn->left;

	if (n->right != NULL)
		n->right->parent = n;

	rn->parent = n->parent;

	if (rn->parent == NULL)
		root = rn;
	else if (rn->parent->left == n)
		n->parent->left = rn;
	else if (rn->parent->right == n)
		n->parent->right = rn;
	else 
		panic("ncwrotate rn parent has no value n");
	
	rn->left = n;
	n->parent = rn;
	(*r) = root;

}

void subrebuild(Node** root, Node* n) {
	Node* pn = NULL;
	Node* ppn = NULL;
	Node* cppn = NULL;
	char left = 0;
	char right = 1;
	char pp_position;
	char color;

	
	while((n->parent) && (n->color == RD) && (n->parent->color == RD)) {
		pn = n->parent;
		ppn = n->parent->parent;

		if (ppn->left == pn) {
			cppn = ppn->right;
			pp_position = left;
		} else {
			cppn = ppn->left;
			pp_position = right;
		}
		
		if (ppn->color == RD) 
			panic("rb policy 3: old tree red red should not be allowed");
		//recoloring
		if (cppn && (cppn->color == RD)) {
			recolor(ppn);
		}

		//rotation
		else {
			if ((pp_position == left) && (pn->left == n)) {
//				printf("ll rotation\n");
				cwrotation(root, ppn); 
				color = pn->color;
				pn->color = ppn->color;
				ppn->color = color;
			} else if ((pp_position == right) && (pn->right == n)) {
//				printf("rr rotation\n");
				ucwrotation(root, ppn);
				color = pn->color;
				pn->color = ppn->color;
				ppn->color = color;
			} else if ((pp_position == left) && (pn->right == n)) {
//				printf("lr rotation\n");
				ucwrotation(root, pn);
				pn = n;
				n = pn->left;
				cwrotation(root, ppn);
				color = pn->color;
				pn->color = ppn->color;
				ppn->color = color;
			} else {
//				printf("rl rotation\n");
				cwrotation(root, pn);
				pn = n;
				n = n->left;
				ucwrotation(root, ppn);
				color = pn->color;
				pn->color = ppn->color;
				ppn->color = color;
			}
			pn = ppn;
			ppn = ppn->parent;
		}
		n = ppn;
	}

	(*root)->color = BK;
}

void 
insert(Node** root, ukey_t key, udata_t data, Output* o) {
	Node* fnode;
	Node* leaf;
	int tp;
	
	o->order = INSERT;
	o->record = (Record*)malloc(sizeof(Record));
	o->record[0].key = key;
	o->record[0].data = data;

	fnode = find_leaf(*root, key, &tp);
	//============================================
	if (fnode == NULL) {
		o->count = 1;
		leaf = make_node(key, data);
		*root = leaf;
		(*root)->color = BK;
		return;
	}
	//update
	if (fnode->key == key) {
		o->odata = fnode->data;
		o->count = 0;
		fnode->data = data;
		return;
	} 

	leaf = make_node(key, data);

	//right
	if (fnode->key > key) {
		fnode->left = leaf;
		leaf->parent = fnode;
	} else {
		//left
		fnode->right = leaf;
		leaf->parent = fnode;
	}
	//============================================
	subrebuild(root, leaf);
	o->count = 1;
	
	if( o->count == 0) {
		//printf("update(%ld): %ld -> %ld \n", o->record[0].key, o->odata, o->record[0].data);
	}
	else {
		//printf("insert(%ld, %ld)\n", o->record[0].key, o->record[0].data);
	}
}

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
		//printf("not found(%ld)!!\n", key);
	}
	else {
		if (o->order == RD) {
			//printf("found(%ld, %ld) on d: %d clr: RED\n", o->record[0].key, o->record[0].data, depth);
		}else{ 
			//printf("found(%ld, %ld) on d: %d clr: BLACK\n", o->record[0].key, o->record[0].data, depth);
		}
	}
}






/*
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
		o->odata = node->data;
		o->count = 0;
		node->data = value;
		return node;	//if null then same key exist
	}
	//set height
	printf("max: %d\n", MAX(height(node->left), height(node->right)));
	printf("lh: %d\n", height(node->left));
	printf("rh: %d\n", height(node->right));
	node->height = MAX(height(node->left), height(node->right)) + 1;
	printf("nodeh: %d\n", node->height);
	//get balance
	hc = height_checker(node);
	printf("hc: %d\n", hc);
	//right is bigger
	if (hc == -1) {
		printf("-1 rotate!!\n");
		if (key > node->right->key) {
			printf("k > n rotate!!\n");
			subroot = node->right;
			//ncwrotate
			ncwrotate(node);
			return subroot; 
		}
		else if (key < node->right->key) {
			printf("k < n rotate!!\n");
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
		printf("1 rotate!!\n");
		if (key < node->right->key) {
			printf("k < n rotate!!\n");
			subroot = node->left;
			//ncwrotate
			cwrotate(node);
			return subroot; 
		}
		else if (key > node->right->key) {
			printf("k > n rotate!!\n");
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
			printf("key: %ld\npkey: %ld\n", key, pnode->key);
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
			o->record[0].data = pnode->data;
			if (pnode->height >= 1) 
				o->height = pnode->height - 1;
			else 
				o->height = pnode->height;
			o->depth = find_info->depth;
			return find_info;
		}
	}	
}
*/
void preOrder(Node* n, int* cur,Output* o) {
	if (n == NULL)
		return;
	//printf("%ld(%d) ", n->key, n->height);
	
	o->record[*cur].key = n->key;
	o->record[(*cur)++].data = n->data;

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
