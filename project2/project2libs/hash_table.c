#include<stdlib.h>
#include"linked_list.h"
#include"double_list.h"
#include"hash_table.h"

void hash_init(hashTable *ht, int length){// initialing the hash table
    int i;
    
    ht->entries = 0;
    ht->min_length = length;
    ht->length = length;
    ht->hash_table = (entry*)malloc(sizeof(entry) * ht->length);
    ht->bucket = (int*)malloc(sizeof(int) * ht->length);
    ht->load_factor = (float)ht->entries / (float)ht->length;
    
    for(i = 0; i < ht->length; i++){
        dlist_init(&(ht->hash_table[i]));
        ht->bucket[i] = 0;
    }
}

unsigned long hash_function(char *str){ //given function by teacher to help find the hash value 
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

int key(unsigned long hash, int lenght){ //returns the hash value
    return (hash%lenght);
}

void hash_add(hashTable *ht, entry *new){ //adds the new entry to the hash table
    int hashIndex; //position at hash table
    
    hashIndex = key(hash_function(new->name), ht->length);
    
    dlist_add(hashIndex, ht, new);
    (ht->bucket[hashIndex])++;
}

void hash_rmv(hashTable *ht, entry *old){ //removes the register (old) from the hash table
    int hashIndex;
    
    hashIndex = key(hash_function(old->name), ht->length);
    
    dlist_rmv(hashIndex, ht, old);
    (ht->bucket[hashIndex])--;
}

void hash_clear(hashTable *ht){ //clears the hash table
    int i;
    
    for(i = 0; i < ht->length; i++)
        dlist_clear(&(ht->hash_table[i]), ht->length);
    
    ht->bucket = (int*)realloc(ht->bucket, sizeof(int) * ht->min_length);
    ht->hash_table = (entry*)realloc(ht->hash_table, sizeof(entry) * ht->min_length);
    ht->length = ht->min_length;
    
    for(i = 0; i < ht->length; i++)
        ht->bucket[i] = 0;
        
    ht->entries = 0;
    ht->load_factor = (float)ht->entries / (float)ht->length;
}

int largest_bucket(hashTable ht){ // this function will get an array of integers abd will return the biggest number
    int i;
    int temp; 
    
    temp = ht.bucket[0];
    
    for(i = 1; i < ht.length; i++){
        if(ht.bucket[i] > temp)
            temp = ht.bucket[i];
    }
    return temp;
}

void rehash(hashTable *ht, array *t, int new_length){
    int i;

    free(ht->bucket);
    free(ht->hash_table);
    ht->length = new_length;
    ht->hash_table = (entry*)malloc(sizeof(entry) * ht->length);
    ht->bucket = (int*)malloc(sizeof(int) * ht->length);
    ht->load_factor = (float)ht->entries / (float)ht->length;
    
    for(i = 0; i < ht->length; i++){
        dlist_init(&(ht->hash_table[i]));
        ht->bucket[i] = 0;
    }
    for(i = 0; i < t->entries; i++)
        hash_add(ht, t->p[i]);
} 
