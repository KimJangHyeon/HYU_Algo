typedef struct _Entry {
	ukey_t key;
	int data;
} Entry;

typedef struct _HashS {
	usize_t size;
	usize_t uprime;
	usize_t dprime;
	Entry * datas;
} HashS;


typedef struct _Hash {
	usize_t size;
	usize_t uprime;
	usize_t dprime;
	HashS * ht;
} Hash;
#define LINEARHASH_C	1
#define SHASH_C			7

#define HASH_EMPTY		-254
#define KEY_EMPTY		-1
#define OUTPUT_VALNULL	-254
#define OUTPUT_IAMQ		-81
void init_hash(Hash*, usize_t, usize_t, ukey_t*);
void operate_hash(Hash*, Order*, Output*);
