/*The descriptions of the following functions are included in the objdb.h headerfile.*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"objdb.h" 

long int file_size(FILE *db){
    long int size;
    
    fseek(db, 0, SEEK_END);
    size = ftell(db);
    
    return size;
}

int Open(FILE **db, char *dbname){ 
    char holder[5];
    int a;
    
    if(*db != NULL)
        fclose(*db);
    
    *db = fopen(dbname, "ab+");
    
    if(*db == NULL)
        return -1;
    a = fread(holder, 1, 5, *db);
    if(ferror(*db) == 0){
        fprintf(stderr, "\nError in Open\n");
    }
    if(a == 0){
        fwrite("base\n", 1, 5, *db);
        return 1; //success
    }
    else if(strcmp("base\n", holder) != 0){
        fclose(*db);
        *db = NULL;
        return 0;
    }
    return 1;
}
  
int Close(FILE **db){
    
    if(*db != NULL){
        fclose(*db);
        *db = NULL;
        return 1;
    }
    else
        return 0;
}

int Import(FILE **db, char *fname, char *name){//ready
    int a, b, o;
    char holder[512];
    FILE *new;
    long int size, found;
    
    found = search(*db, name);
    
    if(*db == NULL)
        return -1;
    else if(found != 0)
        return 1;
    else{
        new = fopen(fname, "rb+");
        if(new == NULL)
            return 0;
        
        b = fwrite(name, sizeof(char), 256, *db); 
        if(ferror(*db) || b !=256){
            fprintf(stderr, "\nError writting the name\n");
            exit(1);
        }
        size = file_size(new);
        
        o = fwrite(&size, sizeof(long), 1, *db);
        if(ferror(*db) || o != 1){
            fprintf(stderr, "\nError writting the size\n");
            exit(1);
        }
    
        fseek(new, 0, SEEK_SET);
        while(1){
            a = fread(holder, sizeof(char), 512, new);
        
            b = fwrite(holder, sizeof(char), a, *db);
            if(ferror(*db) || b != a){
                fprintf(stderr, "\nProblem writting the content\n");
            }
            if(a < 512)
                break;
            }
        fclose(new);
        }
        return 2;
}

long int search(FILE *db, char *name){ // returns 0 if the object isnt in the base file
    long int temp_size, res;
    int a, b;
    char holder[256];
    
    if(db == NULL)
        return -1;
    if(file_size(db) == 5) //empty base so no need of searching in it.
        return 0; 
    else{
        fseek(db, +5L, SEEK_SET); // moves the pointer at the start of the file after the "base"
        while(1){
            b = fread(holder, sizeof(char), 256, db);
            if(b != 256 && a == 0)
                fprintf(stderr, "\nProblem reading the name\n");
            
            if(strcmp(holder, name) == 0){
                res = ftell(db);
                return res;
            }
            
            a = fread(&temp_size, sizeof(long int), 1, db);//reads the size of the file// 
            if(a != 1 && a != 0)
                fprintf(stderr, "\nProblem reading the size\n");
            
            fseek(db, temp_size, SEEK_CUR);
            if(ferror(db)){
                fprintf(stderr, "\nProblem with fseek\n");
                exit(1);
            }
            if(a == 0)
                break;
        }
        return 0; // not found
    }
}

int Find(FILE *db, char *name, char ***objects){
    long int temp_size;
    int a, b, i = 0;
    char holder[256] = {'\0'};

    if(db == NULL)
        return -1;
    
    if(strcmp(name, "*") == 0){
        fseek(db, +5L, SEEK_SET);// moves the pointer at the start of the file      
        *objects = (char**)malloc(sizeof(char*));
        while(1){
            
            b = fread(holder, sizeof(char), 256, db);
            if(b < 256)
                break;
            if(b != 256 && a == 0)
                fprintf(stderr, "\nProblem reading the name\n");
            
            (*objects)[i] = malloc(sizeof(char) * 256);
            strcpy((*objects)[i], holder);
            i++;
            *objects = (char**)realloc(*objects, sizeof(char*) * (i + 1));
            
            a = fread(&temp_size, sizeof(long int), 1, db); //reads the size of the file
            
            if(a != 1 && a != 0)
                fprintf(stderr, "\nProblem reading the size\n");
            
            fseek(db, temp_size, SEEK_CUR);
            if(ferror(db)){
                fprintf(stderr, "\nProblem with fseek\n");
                exit(1);
            }
            if(a == 0)
                break;
        }
        return i;
    }
    else{
        fseek(db, +5L, SEEK_SET); // moves the pointer at the start of the file        
        *objects = (char**)malloc(sizeof(char*));
        while(1){
            
            b = fread(holder, sizeof(char), 256, db);
            if(b < 256)
                break;
            if(b != 256 && a == 0)
                fprintf(stderr, "\nProblem reading the name\n");
            
            if(strstr(holder, name) != NULL){
                (*objects)[i] = malloc(sizeof(char) * 256);
                strcpy((*objects)[i], holder);
                i++;
                *objects = (char**)realloc(*objects, sizeof(char*) * (i + 1));
            }
           
            a = fread(&temp_size, sizeof(long int), 1, db);//reads the size of the file// 
            if(a != 1 && a != 0)
                fprintf(stderr, "\nProblem reading the size\n");
            
            fseek(db, temp_size, SEEK_CUR);
            if(ferror(db)){
                fprintf(stderr, "\nProblem with fseek\n");
                exit(1);
            }
            if(a == 0)
                break;
        }
        return i;
    }
        return 1;
}

int Export(FILE *db, char *name, char *fname){
    char holder[512];
    FILE *new;
    long int size, found, last_add, times;
    int a, b, i = 0;
    
    if(db == NULL)
        return -1;
    
    found = search(db, name);
    if(found == 0 )
        return 0;
    
    else{
        new = fopen(fname, "wb+");
        if(new == NULL)
            return 0;
        fseek(db, found, SEEK_SET);
        a = fread(&size, sizeof(long int), 1, db);
        if(ferror(db) || a != 1){
            fprintf(stderr, "\nProblem reading the size\n");
            exit(1);
        }
        times = size / 512;
        last_add = size %512;
        
        while(1){
            if(i == times){
                a = fread(holder, sizeof(char), last_add, db);
                b = fwrite(holder, sizeof(char), last_add, new);
                if(ferror(db) || a !=b ){
                    fprintf(stderr, "\nProblem reading-writting the content\n");
                    exit(1);
                }
                break;
            }
            a = fread(holder, sizeof(char), 512, db);
            b = fwrite(holder, sizeof(char), a, new);
            i++;
            if(ferror(db)  || b != a)
                fprintf(stderr, "\nError in export\n");
        }
    }
    return 2; // just returning a number so my main won't print anything
}

int Delete(FILE **db, char *name, char *dbname){
    long int found, size, next_size;
    int a, b, l;
    char holder[512], next_name[256];
    long int temp1, temp2, db_size, new_size;
    int writtenbytes = 0, readbytes = 0;
    
    if(*db == NULL)
        return -1;
    fclose(*db);
    
    *db = fopen(dbname, "rb+");
    if(*db == NULL){
        fprintf(stderr, "\nProblem reopening the file\n");
        exit(1);
    }
    found = search(*db, name); 
    if(found == 0)
        return 0;
    
    else{
        db_size = file_size(*db);
        fseek(*db, found, SEEK_SET);// we are about to read the size of the file we want to delete
        temp1 = ftell(*db) - 256; //the posiotion we are writting
        
        l = fread(&size, sizeof(long int), 1, *db);
        if(ferror(*db) || l != 1)
            fprintf(stderr, "\nProblem in delete\n");
        fseek(*db, size, SEEK_CUR);
        if(ferror(*db)){
            fprintf(stderr, "\nProblem in delete (fseek)\n");
            exit(1);
        }
        temp2 = ftell(*db); //the position we are reading from 
        
        while(1){
            b = fread(next_name, sizeof(char), 256, *db); 
            if(temp2 == db_size)//we reached end of file
                break;
            b = fread(&next_size, sizeof(long int), 1, *db);
            if(ferror(*db) || b != 1){
                fprintf(stderr, "\nProblem reading the size\n");
                exit(1);
            }
            temp2 = ftell(*db);
            
            fseek(*db, temp1, SEEK_SET);
            b = fwrite(next_name, sizeof(char), 256, *db); 
            b = fwrite(&next_size, sizeof(long int), 1, *db); 
            temp1 = ftell(*db);
            
            fseek(*db, temp2, SEEK_SET);
            if(ferror(*db)){
                fprintf(stderr, "\nProblem with fseek\n");
                exit(1);
            }

            while(1){
                a = fread(holder, sizeof(char), 512, *db);
                readbytes += a;
                
                if(readbytes < next_size){
                    
                    temp2 = ftell(*db);
                    fseek(*db, temp1, SEEK_SET);
                    b = fwrite(holder, sizeof(char), a, *db); 
                    writtenbytes += b;
                    
                    temp1 = ftell(*db);
                    fseek(*db, temp2, SEEK_SET);
                }
                else{
                    temp2 = ftell(*db) - a + next_size - writtenbytes;
                    fseek(*db, temp1, SEEK_SET);
                    b = fwrite(holder, sizeof(char), next_size - writtenbytes, *db); 
                    
                    temp1 = ftell(*db);
                    fseek(*db, temp2, SEEK_SET);
                    break;
                }                
            }
            writtenbytes = 0;
            readbytes = 0;
        }
        
        new_size = db_size - size - 256 - sizeof(long int);
        fseek(*db, 0, SEEK_END);
        fclose(*db);
        truncate(dbname, new_size);
        *db = fopen(dbname, "ab+");
    } 
    return 1;
}
