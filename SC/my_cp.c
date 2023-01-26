#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define DEFAULT_SIZE 256
#define ERROR -1

typedef char* string;

int main (int argc, string argv[]){
    if(argc<3 || argc >4){
        puts("Errore argomenti");
        return ERROR;
    }

    int fileIn;
    if((fileIn = open(argv[1],O_RDONLY,0666))==-1) {
        perror("Apertura file input");
        exit(EXIT_FAILURE);
    }
    int bufferSize = DEFAULT_SIZE;
    if(argc==4){
        string temp=NULL;
        bufferSize = (int)strtol(argv[3], &temp, 0);
    }
    string buffer = malloc(sizeof(char)*bufferSize);
    int readByte=0;
    if((readByte=read(fileIn, buffer, bufferSize))==0) {
        perror("Lettura");
    } 
    
     int fileOut;
    if((fileOut = open(argv[2], O_WRONLY|O_CREAT, 0666))==-1){
        perror("Creazione file output");
        exit(EXIT_FAILURE);
    }
    if(write(fileOut,buffer,readByte)==-1){
        perror("scrittura");
        exit(EXIT_FAILURE);
        }

    if(close(fileIn)==-1 || close(fileOut)==-1){
        perror("chiusura file");
        exit(EXIT_FAILURE);
    }
    
    free(buffer);

    return 0;
}