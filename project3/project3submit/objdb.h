#ifndef OBJDB
#define OBJDB

extern long int file_size(FILE *db);
/*Calculates the size of the given file.*/

extern int Open(FILE **db, char *dbname);
/*Checks if the given file is a base file. Depends the previous action opens the file or prints an error messege.*/

extern int Import(FILE **db, char *fname, char *name);
/*Adds a new object at the base.*/

extern long int search(FILE *db, char *name);
/*Search the base for the object and returns the position in tha base. If the file doesnt exist prints an error messege*/

extern int Find(FILE *db, char *name, char ***objects);
/*Prints all the objects with the given name in their own name. If the user gives * it prints all the objects.*/

extern int Export(FILE *db, char *name, char *fname);
/*Exports the given object from the base to a new file.*/

extern int Delete(FILE **db, char *name, char *dbname);
/*Deletes the object from the base*/

extern int Close(FILE **db);
/*Closes an open base file.*/

#endif
