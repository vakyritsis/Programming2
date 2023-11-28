#ifndef OBJDB
#define OBJDB

long int file_size(FILE *db);

int Open(FILE **db, char *dbname);

int Import(FILE **db, char *fname, char *name);

long int search(FILE *db, char *name);

int Find(FILE *db, char *name, char ***objects);

int Export(FILE *db, char *name, char *fname);

int Delete(FILE **db, char *name, char *dbname);

int Close(FILE **db);

#endif
