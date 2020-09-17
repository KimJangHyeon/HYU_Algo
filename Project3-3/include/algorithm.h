typedef struct _Node {
	ukey_t key;
	udata_t data;
	Node* next;
	Node* down;
	//Node* up;
} Node;

typedef struct _Header {
	Node* node;
	int height;
} Header;




void insert(Header*, ukey_t, udata_t, Output*);
void find(Header*, ukey_t, Output*);

