
void hash_init(hashTable *ht, int length); 

unsigned long hash_function(char *str);

int key(unsigned long hash, int lenght);

void hash_add(hashTable *ht, entry *new);

void hash_rmv(hashTable *ht, entry *old);

void hash_clear(hashTable *ht);

int largest_bucket(hashTable ht);

void rehash(hashTable *ht, array *t, int new_length);
