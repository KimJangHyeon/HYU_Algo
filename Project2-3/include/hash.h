typedef struct _Entry {
	int data;
	ukey_t key;
} Entry;

typedef struct _Hash {
	usize_t size;
	Entry * datas;
} Hash;

#define LINEARHASH_C	1
#define HASH_EMPTY		-254
#define KEY_EMPTY		-1
#define OUTPUT_VALNULL	-254
#define OUTPUT_IAMQ		-81
#define PRIMEHASH		997
#define HASHJ			7
void hash_insert(Hash*, Hash*, ukey_t, int, Output*);
void init_hash(Hash*, Hash*, usize_t);
void printTable(Hash*, Hash*,  Output*);
void operate_hash(Hash*, Hash*, Order*, Output*);
