typedef struct _Entry {
	int data;
	ukey_t key;
} Entry;

typedef struct _Hash {
	usize_t size;
	Entry * datas;
} Hash;

#define LINEARHASH_C	3
#define HASH_EMPTY		-254
#define KEY_EMPTY		-1
#define OUTPUT_VALNULL	-254
#define OUTPUT_IAMQ		-81
void init_hash(Hash*, usize_t);
void operate_hash(Hash*, Order*, Output*);
void printTable(Hash*, Output*);
