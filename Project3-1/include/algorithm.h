typedef struct _Node {
	ukey_t key;
	udata_t value;
	int height;
	struct _Node* left;
	struct _Node* right;
} Node;

typedef struct _FindInfo {
	char isFind;
	int height;
	int depth;
} FindInfo;

Node* insert(Node*, ukey_t, udata_t, Output*);
FindInfo* find(Node*, ukey_t, Output*);
void printTree(Header*,Output*);

