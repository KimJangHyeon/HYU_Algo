typedef struct _Node {
	ukey_t key;
	udata_t data;
	char color;
	struct _Node* left;
	struct _Node* right;
	struct _Node* parent;
} Node;

void insert(Node**, ukey_t, udata_t, Output*);
void find(Node*, ukey_t, Output*);
void printTree(Header*,Output*);

