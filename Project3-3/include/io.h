typedef struct _Node Node;


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

BufferPool* input_file(char* path);
void output_file(char*, BufferPool*);
void output_memory(BufferPool*);
