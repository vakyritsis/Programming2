/*ONOMATEPWNYMO: VASILEIOS KYRITSIS, AEM: 02999*/
/*This program is about storing in computer's memory infos about the students of a university, such as AEM, their name and how many courses the need to pass*/
/*Now in this program we extent project1 so the computer stores the list of subjects that the students are registered for. Also it stores them in a hash table for quicker access*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

// structs//
typedef struct list { //simple linked list
    unsigned short course_nr;
    struct list *next;
} list;

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


//list functions//
void list_init(list **head){ //init the list
    *head = NULL;
}

int list_find(list *head, unsigned short COURSE_NR){ //search the list for a course number returns 0=1, not found- found
    list *curr;
    
    for(curr = head; (curr != NULL) && (curr->course_nr != COURSE_NR); curr = curr->next);
    
    return (curr != NULL);
}

list *newNode(unsigned short COURSE_NR){ //this function's job is to allocate memory for a node for the course list and to return the adress of the pointer.
    list *curr;
    
    curr = (list*)malloc(sizeof(list));
    
    curr->course_nr = COURSE_NR;
    curr->next = NULL;
    
    return curr;
}

void list_add(list **head, list *new_node){ //inserts the course's number to the students list with increasing order
    list *curr;
    
    if(*head == NULL || (*head)->course_nr >= new_node->course_nr){
        new_node->next = *head; 
        *head = new_node; 
    }
    else{
        curr = *head;
        while(curr->next != NULL && curr->next->course_nr < new_node->course_nr){
            curr = curr->next;
        }
        new_node->next = curr->next; 
        curr->next = new_node; 
    }
}

void list_rmv(list **head, unsigned short COURSE_NR){ //removes the course number from students list
    list *curr, *prev;
    
    for(curr = *head; (curr != NULL) && (curr->course_nr != COURSE_NR); prev = curr, curr = curr->next);
    
    if(curr != NULL){
        if(curr == *head)
            *head = curr->next;
        else
            prev->next = curr->next;
        free(curr);
    }
}

void list_print(list *head){ //for a given student it prints the lessons that he registered for
    list *curr;
    
    curr = head;
    while(curr != NULL){
        printf("%hu\n", curr->course_nr);
        curr = curr->next;
    }
}

void list_clear(list **head){ //clears the list of subjects for a student
    list *curr, *next;
    
    curr = *head;
    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
    *head = NULL;
}


//double list functions//
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


//hash tables functions//
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
        if(ht.bucket[i] > temp){
            temp = ht.bucket[i];
        }
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


//help functions//
int  initialization(array *t, char *argv[]){ //initialing the array of the pointers
    int i;
    t->size = atoi(argv[1]);
    t->resize = atoi(argv[2]);
    t->entries = 0;
    t->sorted = 0;
    
    if(t->size == 0)
        t->p = NULL;
    else{
        t->p = malloc(t->size * sizeof(entry*));
        if(t->p == NULL)
            return 0;
    }
    for(i = 0; i < t->size; i++)
        t->p[i] = NULL;

    return 1;
}

int binary_search(array *t,int l, int r, long unsigned int AEM, int *comp){ //returns the position in pointers array of the given AEM if it's in, otherwise returns -1
    int m;
    
    *comp = 0;
    
    while (l <= r) { 
            m = ((r + l) / 2);
            (*comp)++;
        if (t->p[m]->aem == AEM)
            return m; 
        if (t->p[m]->aem < AEM){ 
            (*comp)++;
            l = m + 1; 
        }
        else{
            (*comp)++;
            r = m - 1; 
        }
    } 
    return -1; 
} 

int linear_search(array *t, long unsigned int AEM, int *comp){ //returns the position in pointers array of the given AEM if it's in, otherwise returns -1
    int i;
    *comp = 0;
    for(i = 0; i < t->entries; i++){
        if(t->p[i]->aem == AEM){
            *comp = i + 1;
            return i;
        }
    }
    *comp = t->entries;
    return -1;
}

int search(array *t, long unsigned int AEM, int *comp){// if the array is sorted we use binary or linear search to find a student using his AEM
    int pos;
    
    if(t->sorted == 1 && t->entries > 2)
        pos = binary_search(t, 0, t->entries-1, AEM, comp);
    
    else
        pos = linear_search(t, AEM, comp);
    
    if(pos != -1)
        return pos;// found
    
    else 
        return -1;// not found
}

void is_sorted(array *t){// checks if the array is sorted
    int i;
    
    for(i = 0; i < t->entries - 1; i++){
        if(t->p[i]->aem > t->p[i + 1]->aem){
            t->sorted = 0;
            break;
        }
        else 
            t->sorted = 1;
    }
}

void allUpper(char s[]){// converts every letter of the name of the student to capital 
    int i;
    
    for(i = 0; s[i] != '\0'; i++){
        if(s[i] >= 'a' && s[i] <= 'z')
            s[i] = s[i] - 32;
    }
}

//menu functions//
int add(long unsigned int AEM, char NAME[], short unsigned int COURSES, array *t, int *comp, hashTable *ht){// checks if the given AEM is in the array, if not add the the student in the array
    int pos, i;
    entry *ptr;
    
    if(t->entries > 0){
        pos = search(t, AEM, comp);
        if(pos != -1){
            return -1;
        }
    }
    if(t->size == t->entries){
        t->p = (entry**)realloc(t->p, (t->size + t->resize) * sizeof(entry*));
        for(i = 0; i < t->resize; i++)
            t->p[i + t->size] = NULL;
        t->size += t->resize;
    }
    ptr = malloc(sizeof(entry));
    
    if(ptr == NULL)
        return 0;
    
    t->p[t->entries] = ptr;
    t->p[t->entries]->aem = AEM;
    strcpy(t->p[t->entries]->name, NAME);
    t->p[t->entries]->courses = COURSES;
    list_init(&t->p[t->entries]->head);
    
    is_sorted(t);

    hash_add(ht, ptr);
    
    t->entries++;
    ht->entries++;
    ht->load_factor = (float)ht->entries / (float)ht->length;
    
    if(ht->load_factor >= 4)
        rehash(ht, t, ht->length * 2);
    
    return 1;
}

int rmv(long unsigned int AEM, array *t, int *comp, hashTable *ht){ //checks if the given AEM is in our array, if yes it removes it otherwise returns -1
    int pos;
    
    pos = search(t, AEM, comp);
    if(pos == -1)
        return 0;
    
    hash_rmv(ht, t->p[pos]);
    
    free(t->p[pos]);
    t->entries--;
    ht->entries--;
    ht->load_factor = (float)ht->entries / (float)ht->length;
    list_clear(&t->p[pos]->head);
    t->p[pos] = t->p[t->entries];
    
    if(t->size - t->entries > t->resize){
        t->p = realloc(t->p, (t->size - t->resize) * sizeof(entry*));
        t->size -= t->resize;
    }
    
    if(t->p == NULL)
        return 0;
    
    is_sorted(t);
    
    if(ht->load_factor == 1 && ht->length > ht->min_length)
        rehash(ht, t, ht->length / 2);
    
    return 1;
}

void print(hashTable ht){ //prints the hash table
    int i;
    
    printf("\n#\n");
    printf("%d %d %.2f %d\n", ht.length, ht.entries, ht.load_factor, largest_bucket(ht));
    
    for(i = 0; i < ht.length; i++){
        printf("%d %d", i, ht.bucket[i]);
        dlist_print(ht.hash_table[i]);
        printf("\n\n");
    }
}

int find(long unsigned int AEM, char NAME[64], short unsigned int *COURSES, array *t, int *comp){ //using function search, checks if a student is in the array, if yes return his position, their courses and their name
    int pos;
    
    pos = search(t, AEM, comp);
    
    if(pos != -1){
        strcpy(NAME, t->p[pos]->name);
        *COURSES = t->p[pos]->courses;
        return pos;
    }
    else
        return pos;
}

int mod(long unsigned int AEM, short unsigned int COURSES, array *t, int *comp){ //changes the remaining courses of a student if the given AEM exists
    int pos;
    
    pos = search(t, AEM, comp);
    
    if(pos != -1){
        t->p[pos]->courses = COURSES;
        return 1;
    }
    else 
        return -1;      
}

int sort(array *t){ //sorts the array putting the smaller AEM first 
    int i, j, comp = 0, change;
    entry *key;
    
    for (i = 1; i < t->entries; i++) { 
        key = t->p[i]; 
        j = i - 1; 
        comp++;
        while (j >= 0 && t->p[j]->aem > key->aem) { 
            comp++;
            t->p[j + 1] = t->p[j]; 
            j = j - 1;
            change = 1;
        } 
        if(change == 1)
            comp--;
        t->p[j + 1] = key; 
    }
    t->sorted = 1;
    
    return comp;
} 

void clear(array *t, hashTable *ht){ //clears the array and free the space 
    int i;
    
    if(t->entries  > 0){
        for(i = 0; i < t->entries; i++){
            list_clear(&t->p[i]->head);
            free(t->p[i]->head);
            free(t->p[i]);
        }
        t->entries = 0;
        t->size = 0;
    }
    hash_clear(ht);
}

int reg(array *t, long unsigned int AEM, unsigned short COURSE_NR){ //returns 1 for success, 0 if AEM doesn't exists and -1 if he is already registered
    int pos, comp, a;
    list *new_node;
    
    pos = search(t, AEM, &comp);
    
    if(pos != -1){
        a = list_find(t->p[pos]->head, COURSE_NR);
            if(a == 0){
                new_node = newNode(COURSE_NR);// here we allocate mememory for the new addition to the list
                list_add(&t->p[pos]->head, new_node);
                return 1;
            }
            else return 0;
    }
    else
        return -1;
}

int unreg(array *t, long unsigned int AEM, unsigned short COURSE_NR){ //returns 1 for success, 0 if AEM doesn't exists and -1 if he is already registered
    int pos, comp, a;
    
    pos = search(t, AEM, &comp);
    
    if(pos != -1){
        a = list_find(t->p[pos]->head, COURSE_NR);
            if(a == 1){
                list_rmv(&t->p[pos]->head, COURSE_NR);
                return 1;
            }
            else
                return 0;
    }
    else
        return -1;
}

int isreg(array t, long unsigned int AEM, unsigned short COURSE_NR){ // if AEM doesn't exists return -1. Returns 1 if AEM exists and the student is registered in the course, otherwise returns 0
    int pos, comp, a;
    
    pos = search(&t, AEM, &comp);
    
    if(pos != -1){
        a = list_find(t.p[pos]->head, COURSE_NR);
            if(a == 1)
                return 1;
            else
                return 0;
    }
    else 
        return -1;
}

void list_courses(array t, long unsigned int AEM){ //prints the list of subjects for the student with the given AEM
    int pos, comp;
    
    pos = search(&t, AEM, &comp);
    
    if(pos != -1){
        printf("\nL-OK %s\n", t.p[pos]->name);
        list_print(t.p[pos]->head);
    }
    else 
        printf("\nL-NOK %lu\n", AEM);
}

void find_by_name(hashTable ht, char NAME[]){ // be given a name and search for it in the hash table, then for every student with this name prints their aem and the remaining courses
    entry *curr;
    int hashIndex;
    int check = 0;
    struct timespec start, end;
    unsigned long elapsed_time;
    int comp = 0;;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    hashIndex = key(hash_function(NAME),  ht.length); 
    
    curr = ht.hash_table[hashIndex].next;
    
    while(curr->aem != ht.hash_table[hashIndex].aem){
        comp++;
        if(strcmp(curr->name, NAME) == 0){
            printf("\nN-OK %s\n", NAME);
            check = 1;
            break;
        }
        curr = curr->next;
    }
    if(check == 0)
        printf("\nN-NOK %s\n", NAME);
    else{
        while(strcmp(curr->name, NAME) == 0){
            printf("%lu %hu\n", curr->aem, curr->courses);
            curr = curr->next;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    fprintf(stderr, "\n%lu microseconds\n", elapsed_time);
    fprintf(stderr, "\n$%d comparisons\n", comp);
}


//main//
int main(int argc, char *argv[]){
    array t; // t stands for table(quick access - easy reading)
    int a, i, comp;
    char choice;
    long unsigned int holder1;//aem
    char holder2[64];//name 
    short unsigned int holder3;//courses
    unsigned short holder4;//course number
    hashTable ht; // stands for hash table
    
    a = initialization(&t, argv);
    if(a == 0)
        return 0;
    
    hash_init(&ht, atoi(argv[3]));
    
    do{
        scanf(" %c", &choice);
        comp = 0;
        holder1 = 0;
        for(i = 0; i < 64 && holder2[i] != '\0'; i++)
            holder2[i] = '\0';
        holder3 = 0;
        holder4 = 0;
        a = 0;
        switch(choice){
            case 'a': {
                scanf("%lu %63s %hu", &holder1, holder2, &holder3);
                allUpper(holder2);
               
                a = add(holder1, holder2, holder3, &t, &comp, &ht);
                                    
                if(a == 1)
                    printf("\nA-OK %lu, %d %d\n", holder1, t.entries, t.size);
                else
                    printf("\nA-NOK %lu, %d %d\n", holder1, t.entries, t.size);
                break;
            }
            case 'r': {
                scanf(" %lu", &holder1);
                a = rmv(holder1, &t, &comp, &ht);
                
                if(a == 1)
                    printf("\nR-OK %lu, %d %d\n", holder1, t.entries, t.size);
                else
                    printf("\nR-NOK %lu, %d %d\n", holder1, t.entries, t.size);
                break;
            }
            case 'm': {
                scanf(" %lu %hu", &holder1, &holder3);
                a = mod(holder1, holder3, &t, &comp);
                
                if(a == 1)
                    printf("\nM-OK %lu\n", holder1);
                else
                    printf("\nM-NOK %lu\n", holder1);
                break;
            }
            case 's': {
                comp = sort(&t);
                printf("\nS-OK\n");
                fprintf(stderr, "\n$%d\n", comp);
                break;
            }
            case 'f': {
                scanf(" %lu", &holder1);
                a = find(holder1, holder2, &holder3, &t, &comp);
                
                if(a != -1){
                    printf("\nF-OK %s %hu\n", holder2, holder3);
                    fprintf(stderr, "\n$%d\n", comp);
                }
                else{
                    printf("\nF-NOK %lu\n", holder1);
                    fprintf(stderr, "\n$%d\n", comp);
                }
                break;
            }
            case 'p': {
                print(ht);
                break;
            }
            case 'c': {
                clear(&t, &ht);
                printf("\nC-OK\n");
                break;
            }
            case 'q': {
                clear(&t, &ht);
                free(t.p);
                free(ht.bucket);
                free(ht.hash_table);
                return 0;
                break;
            }
            case 'g': {
                scanf("%lu %hu", &holder1, &holder4);
                
                a = reg(&t, holder1, holder4);
                
                if(a == 1)
                    printf("\nG-OK %lu %hu\n", holder1, holder4);
                else{
                    if(a == -1)
                        printf("\nG-NOK %lu\n", holder1);
                    else
                        printf("\nG-NOK %hu\n", holder4);
                }
                break;
            }
            case 'u': {
                 scanf("%lu %hu", &holder1, &holder4);
                 
                 a = unreg(&t, holder1, holder4);
                 
                 if(a == 1)
                     printf("\nU-OK %lu %hu\n", holder1, holder4);
                 else{
                     if(a == -1)
                         printf("\nU-NOK %lu\n", holder1);
                    else
                        printf("\nU-NOK %hu\n", holder4);
                 }
                break;
            }
            case 'i': {
                scanf("%lu %hu", &holder1, &holder4);
                 
                a = isreg(t, holder1, holder4);
                 
                if(a != -1){
                    if(a == 1)
                        printf("\nYES\n");
                    else
                        printf("\nNO\n");
                }
                else 
                    printf("\nI-NOK %lu\n", holder1);
                 
                 break;
            }
            case 'l': {
                scanf("%lu", &holder1);
                
                list_courses(t, holder1);
                break;
            }
            case 'n': {
                scanf("%64s", holder2);
                allUpper(holder2);
                find_by_name(ht, holder2);
                break;
            }
        }
    }
    while(1);
    
    return 0;
}
