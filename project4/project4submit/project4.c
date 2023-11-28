#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
/* This program is a version of autolab. Gets as arguments the code of a program(.c), a file with the arguments, a file with the input and a file with the expected output.
 * Finds the total grade and prints the grade for compilation, output and memoryAccess.*/

int compile(char err[]){
    FILE *f;
    int check; 
    char *buffer;
    long int size;
    
    f = fopen(err, "r");
    if(f == NULL){
        fprintf(stderr, "Error reopening the error file\n");
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = (char*)malloc(sizeof(char) * (size + 1));
    check = fread(buffer, sizeof(char), size, f);
    
    if(check == 0)
        return 1; // compiled successfully
    else if(strstr(buffer, "error:") != NULL)
        return -1; // not compiled, found errors
    else if(strstr(buffer, "warning:") != NULL)
        return 0; // compiled with warnings
    
    free(buffer);    
    fclose(f);
    
    return 1;
}

char **find_arguments(char *args, int *numOfArgs, char *executable){ //find the arguments for the program that we are testing
    FILE *f;
    int i = 1, res = 0;
    char **arguments = NULL;
    
    f = fopen(args, "r");
    if(f == NULL){
        fprintf(stderr, "ERROR OPENING ARGUMENTS FILE\n");
        exit(1);
    }
    arguments = (char**)malloc(sizeof(char*));
    arguments[0] = executable;
    
    int cnt = 1;
    char c;
    int is_first = 1;

    while(!feof(f) && cnt > 0){
        for (cnt = 0, c = '\0'; c != ' ' && c != '\n'; cnt++){
            res = fread(&c, 1, 1, f);
            
            if(res == 0 || c == '\n')
                break;
        }
        fseek(f, (is_first)? 0:(-cnt) , (is_first)? SEEK_SET: SEEK_CUR);
        is_first = 0;
        if(cnt > 0) {
            arguments = realloc(arguments, sizeof(char*) * (i + 1)); 
            arguments[i] = (char*)malloc(sizeof(char)* cnt);
            fscanf(f, "%s", arguments[i]);
            
            if(strcmp(arguments[i], "\0") == 0)
                break;
//             printf("-%s-\n", arguments[i]);
            i++;
        }
    }

    arguments[i] = NULL;
    fclose(f);
    *(numOfArgs) = i;
     
    return arguments;
}

void print_grade(int compilation, int output, int memoryAccess){ //prints the grade
    int total;
    
    total = compilation + output + memoryAccess;
    
    if(total < 0)
        total = 0;
    
    printf("Compilation: %d\nOutput: %d\nMemory access: %d\nTotal: %d\n", compilation, output, memoryAccess, total);
}

int main(int argc, char *argv[]){
    char *code = NULL, *args = NULL, *in = NULL, *out = NULL;
    char **arguments;
    int len, fd1, p1, status1, success, numOfArgs, i, compilation, output, memoryAccess = 0;
    int p2, status2;
    int p3 = 1, status3;
    int fd[2];

    if(argc != 5){
        printf("\nWrong number of arguments\n<progname.c> <progname.args> <progname.in> <progname.out>\n");
        return 2;
    }

    code = argv[1];
    args = argv[2];
    in = argv[3];
    out = argv[4];
    
    len = strlen(code);
        
    char executable[len - 2];
    char err[len + 2];
    char command[len];
    
    strcpy(executable, code);
    executable[len - 2] = '\0';
    executable[len - 1] = '\0';
    
    strcpy(err, executable);
    strcat(err, ".err");
    
    strcpy(command, "./");
    strcat(command, executable);
    
    /* P1 */
    
    if(!(p1 = fork())){
        fd1 = open(err, O_RDWR|O_CREAT|O_EXCL ,S_IRWXU);
        
        if(fd1 == -1){
            fprintf(stderr, "Problem creating the error file\n");
            exit(1);
        }
        dup2(fd1, STDERR_FILENO);
        close(fd1);
        execl("/usr/bin/gcc", "gcc", "-Wall", code, "-o", executable, NULL);
    }
    
    waitpid(p1, &status1, 0);
    success = compile(err);
    if(success == -1){
        print_grade(-100, 0, 0);
        return 0;
    }
    else if(success == 0)
        compilation = -10;
    
        
    pipe(fd);
    /*     P2 fork     */
    p2 = fork();
    if(p2 == 0){
        arguments = find_arguments(args, &numOfArgs, executable);
        
        fd1 = open(in, O_RDWR, S_IRWXU);
        dup2(fd1, STDIN_FILENO);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd1);
        close(fd[0]);
        
        execv(command, arguments);
        
        for(i = 0; i < numOfArgs; i++)
            free(arguments[i]);
        free(arguments);
        
        return 1;
    }
    else{
        /*P3 fork         */
        p3 = fork();
        
        if(p3 == 0){
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);

            execl("./p4diff", "p4diff", out, NULL);

            return 1;
        }
    }
    
    close(fd[0]);
    close(fd[1]);
    
    waitpid(p2, &status2, 0);

    if(WIFSIGNALED(status2)){
        if(WTERMSIG(status2) == 6 || WTERMSIG(status2) == 7 || WTERMSIG(status2) == 11)
            memoryAccess = -15;
    }
    
    waitpid(p3, &status3, 0);
    output = WEXITSTATUS(status3);
    print_grade(compilation, output, memoryAccess);
    
    return 0;
}
