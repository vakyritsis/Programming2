typedef struct entry { //struct about the student
    long unsigned int aem;
    char name[64];
    short unsigned int courses;
    list *head;
    struct entry *next;
    struct entry *prev;
} entry;

typedef struct array { //struct about the array 
    entry **p; //pointer to pointer's array
    int size;
    int resize;
    int entries;
    int sorted;
} array;

typedef struct hashTable{
    entry *hash_table;
    int length;
    int min_length; //the minimum length that the hash table can have
    int *bucket;
    int entries;
    float load_factor;
}hashTable;

void dlist_init(entry *head);

void dlist_add(int pos, hashTable *ht, entry *new);

void dlist_rmv(int pos, hashTable *ht, entry *old);

void dlist_clear(entry *head, int length);

void dlist_print(entry head);
