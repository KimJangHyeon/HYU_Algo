
#define MAX_BUF		4096 * 200
typedef struct _Order {
    char func;
    int param1;
    int param2;
} Order;

typedef struct _Output {
    char ret[11];
    int val;
} Output;

typedef struct _Buffer {
    Order order;
    Output output;
} Buffer;

typedef struct _BufferPool {
    Buffer buffers[MAX_BUF];
    int count;
} BufferPool;

BufferPool* input_file(char* path);
void output_file(char*, BufferPool*);

