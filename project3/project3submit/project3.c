/*This program creates a binary file that contains digital objects. The user is able to to add, search, export and delete any object in/from this digital base.*/
#include<stdio.h>
#include<stdlib.h>
#include"objdb.h" 


int main(int argc, char *argv[]){
    int res, i;
    char choice;
    FILE *db = NULL;
    char dbname[256], fname[256], name[256], **objects = NULL;

    do{
        scanf("%c", &choice);
        
        switch(choice){
            case 'o': {
                scanf("%s", dbname);
                res = Open(&db, dbname);
                if(res == -1)
                    printf("\nError opening %s.\n", dbname);
                else if(res == 0)
                    printf("\nInvalid db file %s.\n", dbname);
                break;
            }
            case 'i': {
                scanf("%s %s", fname, name);
                res = Import(&db, fname, name);
                if(res == -1)
                    printf("\nNo open db file.\n");
                else if(res == 1)
                    printf("\nObject %s already in db.\n", name);
                else if(res == 0)
                    printf("\nFile %s not found.\n", fname);
                break;
            } 
            case 'f': {
                scanf("%s", name);                    
                res = Find(db, name, &objects);
                if(res == -1)
                    printf("\nNo open db file.\n");
                else{
                    printf("\n##\n");
                    for(i = 0; i < res; i++)
                        printf("%s\n", objects[i]);
                    for(i = 0; i < res; i++)
                        free(objects[i]);
                    free(objects);
                }
                break;
            }
            case 'e': {
                scanf("%s %s", name, fname);
                res = Export(db, name, fname);
                if(res == 0)
                    printf("\nObject %s not in db.\n", name);
                else if(res == 1)
                    printf("\nCannot open file %s.\n", fname);
                else if(res == -1)
                    printf("\nNo open db file.\n");
                break;
            }
            case 'd': { 
                scanf("%s", name);
                res = Delete(&db, name, dbname);
                if(res == 0)
                    printf("\nObject %s not in db.\n", name);
                else if(res == -1)
                    printf("\nNo open db file.\n");
                break;
            }
            case 'c': {
                res = Close(&db);
                if(res == 0)
                    printf("\nNo open db file.\n");
                break;
            }
            case 'q': {
                if(db != NULL)
                    fclose(db);   
                return 0;
            }
        }
    }
    while(1);
    
    return 0;
}
