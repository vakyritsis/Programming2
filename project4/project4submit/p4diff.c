#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>      
#include<fcntl.h>

int max(char *output, int actualBytes){
    FILE *f;
    long int expectedBytes;
    
    f = fopen(output, "r");
    fseek(f, 0, SEEK_END);
    expectedBytes = ftell(f);
    
    fclose(f);
    
    if(expectedBytes == 0 && actualBytes == 0)
        return -1; // we are returning -1 to point that we dont have any output so the grade will be 100 
    else if(expectedBytes > actualBytes && (actualBytes != 0 || expectedBytes != 0))
        return expectedBytes;
    else 
        return actualBytes;    
}

int main(int argc, char *argv[]){
    char *output = NULL;
    char bufferFromPipe[64] = {0}, bufferFromFile[64] = {0}; 
    int readFromFile = 0, readFromPipe = 0, i, sameBytes = 0, score, actualBytes = 0;
    FILE *f;
    int denominator;
    
    output = argv[1];

    f = fopen(output, "r");
    
    while(1){

        readFromPipe = fread(bufferFromPipe, sizeof(char), 64, stdin);
        
        readFromFile = fread(bufferFromFile, sizeof(char), 64, f);
        
        actualBytes += readFromPipe;

        if (readFromFile >= readFromPipe) {
            for(i = 0; i < readFromPipe; i++){  
                if(bufferFromFile[i] == bufferFromPipe[i])
                    sameBytes++;
            }
        }
        else {
            for(i = 0; i < readFromFile; i++){ 
                if(bufferFromFile[i] == bufferFromPipe[i])
                    sameBytes++;
            }
        }

        if(readFromFile == 0 || readFromPipe == 0){
            
            break;
        }
    }
    fclose(f);
    
    denominator = max(output, actualBytes); 
    if(denominator == -1)
        score = 100;
    else
        score  = (sameBytes * 100) / denominator; 

    return score;
}
