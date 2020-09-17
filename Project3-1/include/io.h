#define MAX_BUF     4096 * 2000
#define PRINTTREE	0
#define FIND		1
#define INSERT		2
#define NOTFOUND	3
#define NONE 		-1

typedef struct _Node Node;

typedef struct _Header {
	Node* node;
	int count;
	int cur;
} Header;

typedef struct _Order {
    char func;
    ukey_t param1;
    udata_t param2;
} Order;

typedef struct _Record {
	ukey_t key;
	udata_t data;
} Record;

typedef struct _Output {
	Record* record;
	udata_t odata;
	int depth;
	int height; 	//avl
	//int color;	//read black
	int count;
	int order;
} Output;

typedef struct _Buffer {
    Order order;
    Output output;
} Buffer;

typedef struct _BufferPool {
    Buffer buffers[MAX_BUF];
} BufferPool;

ukey_t input_key_file(char*, ukey_t**);
BufferPool* input_file(char* path);
void output_file(char*, BufferPool*);

