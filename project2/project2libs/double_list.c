#include<stdio.h>
#include<string.h>
#include"linked_list.h"

typedef struct entry { //struct about the student
    long unsigned int aem;
    char name[64];
    short unsigned int courses;
    list *head;
    struct entry *next;
    struct entry *prev;
} entry;

typedef struct hashTable{
    entry *hash_table;
    int length;
    int min_length; //the minimum length that the hash table can have
    int *bucket;
    int entries;
    float load_factor;
} hashTable;

void dlist_init(entry *head){ //initialing the list with memory allocation for list's head and by making next and prev pointers show the head
    head->aem = 0;
    head->prev = head;
    head->next = head;  
}

void dlist_add(int pos, hashTable *ht, entry *new){ //adds the new entry to the double list
    entry *curr;
    
    curr = ht->hash_table[pos].next;
    
    while(1){
        if((curr->aem == ht->hash_table[pos].aem || strcmp(new->name, curr->name) < 0) || (strcmp(new->name, curr->name) == 0 && new->aem < curr->aem)){
            new->next = curr;
            new->prev=curr->prev;
            new->next->prev=new;
            new->prev->next=new;
            break;
        }
        curr  = curr->next;
    }    
}

void dlist_rmv(int pos, hashTable *ht, entry *old){ //removes the register(old) for the double list
    entry *curr;
    
    curr = ht->hash_table[pos].next;
    while(1){
            if(strcmp(curr->name, old->name) == 0)
                break;
            curr = curr->next;
        }
    curr->next->prev = curr->prev;
    curr->prev->next = curr->next;       
}

void dlist_clear(entry *head, int length){ //clears the double list
    head->next = head;
    head->prev = head;
}

void dlist_print(entry head){ // take as argument the head of a list and print the hole dlist
    entry curr = head;

    while(curr.next->aem != head.aem){
        printf(" [%lu %s %hu]", curr.next->aem, curr.next->name, curr.next->courses);
        curr = *(curr.next);
    }
} 
