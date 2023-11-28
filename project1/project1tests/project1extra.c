/*ONOMATEPWNYMO : VASILEIOS KYRITSIS, AEM : 02999
 *This program is about storing in computer's memory infos about the students of a university, such as AEM, their name and how many courses the need to pass*/
//with time 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

// structs
typedef struct entry {// struct about the student
    long unsigned int aem;
    char name[64];//////64  
    short unsigned int courses;
} entry;

typedef struct array {// struct about the array 
    entry **p;// pointer to pointer's array
    int size;
    int resize;
    int entries;
    int sorted;
} array;

//functions
int binary_search(array *t,int l, int r, long unsigned int AEM, int *comp){ //returns the position in pointers array of the given AEM if it's in, otherwise returns -1
    int m;
    
    *comp = 0;
    
    while (l <= r) { 
            m = ((r + l) / 2);
            (*comp)++;
        if (t->p[m]->aem == AEM){
            
            return m; 
        }
        if (t->p[m]->aem < AEM){ 
            (*comp)++;
            l = m + 1; 
        }
        else{
            (*comp)++;
            r = m - 1; 
        }
    } 
    //(*comp)++;
    return -1; 
} 

int linear_search(array *t, long unsigned int AEM, int *comp){ //returns the position in pointers array of the given AEM if it's in, otherwise returns -1
    int i;
    
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
    
    if(t->sorted == 1 && t->entries > 2){
        pos = binary_search(t, 0, t->entries-1, AEM, comp);
    }
    else{
        pos = linear_search(t, AEM, comp);
    }
    
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

int add(long unsigned int AEM, char NAME[], short unsigned int COURSES, array *t, int *comp){// checks if the given AEM is in the array, if not add the the student in the array
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
        for(i = 0; i < t->resize; i++){
            t->p[i + t->size] = NULL;
        }
        t->size += t->resize;
    }
    ptr = malloc(sizeof(entry));
    
    if(ptr == NULL)
        return 0;
    
    t->p[t->entries] = ptr;
    t->p[t->entries]->aem = AEM;
    strcpy(t->p[t->entries]->name, NAME);
    t->p[t->entries]->courses = COURSES;
    t->entries++;
    
    is_sorted(t);

    return 1;
}

void allUpper(char s[]){// converts every letter of the name of the student to capital 
    int i;
    
    for(i = 0; s[i] != '\0'; i++){
        if(s[i] >= 'a' && s[i] <= 'z')
            s[i] = s[i] - 32;
    }
}

int rmv(long unsigned int AEM, array *t, int *comp){ //checks if the given AEM is in our array, if yes it removes it otherwise returns -1
    int pos;
    
    pos = search(t, AEM, comp);
    if(pos == -1)
        return 0;
    
    
    free(t->p[pos]);
    t->entries--;
    
    t->p[pos] = t->p[t->entries];
    
    if(t->size - t->entries > t->resize){
        t->p = realloc(t->p, (t->size - t->resize) * sizeof(entry*));
        t->size -= t->resize;
    }
    
    if(t->p == NULL)
        return 0;
    
    is_sorted(t);
    
    return 1;
}

void print(array *t){ // printing out all the students in the array with their infos
    int i;
    
    printf("\n#\n");
        for(i = 0; i < t->entries; i++){
            printf("%lu %s %hu\n", t->p[i]->aem, t->p[i]->name, t->p[i]->courses);
        }
}

int find(long unsigned int AEM, char NAME[64], short unsigned int *COURSES, array *t, int *comp, unsigned long *elapsed_time){ //using function search, checks if a student is in the array, if yes return his position, their courses and their name
    int pos;
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    pos = search(t, AEM, comp);
    
    if(pos != -1){
        strcpy(NAME, t->p[pos]->name);
        *COURSES = t->p[pos]->courses;
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        *elapsed_time = (end.tv_sec - start.tv_sec) * 1000000
                + (end.tv_nsec - start.tv_nsec) / 1000;
        return pos;
    }
    else{
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        *elapsed_time = (end.tv_sec - start.tv_sec) * 1000000
                + (end.tv_nsec - start.tv_nsec) / 1000;
        return pos;
    }
}

int mod(long unsigned int AEM, short unsigned int COURSES, array *t, int *comp){ //changes the remaining courses of a student if the given AEM exists
    int pos;
    
    pos = search(t, AEM, comp);
    
    if(pos != -1){//found
        t->p[pos]->courses = COURSES;
        return 1;
    }
    else 
        return -1;      
}

int sort(array *t, unsigned long *elapsed_time){ //sorts the array putting the smaller AEM first 
    int i, j, comp = 0, change;
    entry *key;
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
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
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    *elapsed_time = (end.tv_sec - start.tv_sec) * 1000000
                + (end.tv_nsec - start.tv_nsec) / 1000;
    
    return comp;
} 

void clear(array *t){ //clears the array and free the space 
    int i;
    
    if(t->entries  > 0){
        for(i = 0; i < t->entries; i++){
            free(t->p[i]);
        }
        t->entries = 0;
        t->size = 0;
    }
}

//main
int main(int argc, char *argv[]){
    array t; // t stands for table(quick access - easy reading)
    int a, b, c, d, i, comp;
    char choice;
    long unsigned int holder1;//aem
    char holder2[64];//name 
    short unsigned int holder3;//courses
    unsigned long elapsed_time;
    
    t.size = atoi(argv[1]);
    t.resize = atoi(argv[2]);
    t.entries = 0;
    t.sorted = 0;
   
    if(t.size == 0){
        t.p = NULL;
    }
    else{
        t.p = malloc(t.size * sizeof(entry*));
        if(t.p == NULL)
            return 0;
    }
    for(i = 0; i < t.size; i++){
        t.p[i] = NULL;
    }
    
    do{
        scanf(" %c", &choice);
        comp = 0;
        elapsed_time = 0;
        
        switch(choice){
            case 'a': {
                holder1 = 0;
                for(i = 0; i < 64; i++)
                    holder2[i] = '\0';
                holder3 = 0;
                scanf("%lu %63s %hu", &holder1, holder2, &holder3);
                allUpper(holder2);
               
                a = add(holder1, holder2, holder3, &t, &comp);
                                    
                
                if(a == 1){
                    printf("\nA-OK %lu, %d %d\n", holder1, t.entries, t.size);
                }
                else{
                    printf("\nA-NOK %lu, %d %d\n", holder1, t.entries, t.size);
                    break;
                }
                break;
            }
            case 'r': {
                scanf(" %lu", &holder1);
                b = rmv(holder1, &t, &comp);
                
                if(b == 1){
                    printf("\nR-OK %lu, %d %d\n", holder1, t.entries, t.size);
                }
                else{
                    printf("\nR-NOK %lu, %d %d\n", holder1, t.entries, t.size);
                }
                break;
            }
            case 'm': {
                scanf(" %lu %hu", &holder1, &holder3);
                d = mod(holder1, holder3, &t, &comp);
                
                if(d == 1){
                    printf("\nM-OK %lu\n", holder1);
                }
                else{
                    printf("\nM-NOK %lu\n", holder1);
                }
                
                break;
            }
            case 's': {
                comp = sort(&t, &elapsed_time);
                
                printf("\nS-OK\n");
                fprintf(stderr, "\n$%d\n", comp);
                fprintf(stderr, "%lu microseconds\n", elapsed_time);
                break;
            }
            case 'f': {
                scanf(" %lu", &holder1);
                c = find(holder1, holder2, &holder3, &t, &comp, &elapsed_time);
                
                if(c != -1){
                    printf("\nF-OK %s %hu\n", holder2, holder3);
                    fprintf(stderr, "\n$%d\n", comp);
                    fprintf(stderr, "%lu microseconds\n", elapsed_time);
                }
                else{
                    printf("\nF-NOK %lu\n", holder1);
                    fprintf(stderr, "\n$%d\n", comp);
                    fprintf(stderr, "%lu microseconds\n", elapsed_time);
                }
                break;
            }
            case 'p': {
                print(&t);
                break;
            }
            case 'c': {
                
                clear(&t);
                printf("\nC-OK\n");
                
                break;
            }
            case 'q': 
                clear(&t);
                return 0;
                break;
        }
    }
    while(1);

    return 0;
}
