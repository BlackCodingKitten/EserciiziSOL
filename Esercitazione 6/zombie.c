#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>


typedef char* string;

int toNumber(string n);
void exitMessage (void);

int main (int argc, string argv[]){
    if(argc!=2){
        fprintf(stderr, "Inserire il numero di processi zombie da creare");
        exit(EXIT_FAILURE);
    }  
    int n=toNumber(argv[1]);
    for(int i=0; i<n; i++){
        pid_t pid = fork();
        if(pid > 0){
            sleep(15);
        }
        else{
            exit(EXIT_SUCCESS);
        }
    }

    
    return 0;
}

int toNumber(string s){
    string e=NULL;
    int num =strtol(s,&e,0);
    if(e!=NULL && *e == (char)0){
        return num;
    }
    fprintf(stderr, "Numero non valido");
    exit(EXIT_FAILURE);
}

void exitMessage (void){
    puts("chiusura in corso");
}