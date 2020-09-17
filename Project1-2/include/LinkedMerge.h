
typedef struct _Entry {
    int data;
    struct _Entry* next;
} Entry;
typedef struct _LinkedMerge {
    unsigned int count;
    Entry* front;
    Entry* rear;
} LinkedMerge;

typedef struct _MergeArray {
	char check;
	int count;
    LinkedMerge * merge_arr;
} MergeArray;

typedef struct _Data {
	FILE *fp;
	LinkedMerge* lm;
} Data;


Entry* make_entry(int);
void init_linked_merge(LinkedMerge*);
void insert_back(LinkedMerge*, int);
Entry* pop_lm(LinkedMerge*);
void print_lm(LinkedMerge*);
void print_ma(MergeArray*);
void linked_merge_conquer(LinkedMerge*, LinkedMerge*, LinkedMerge*);

