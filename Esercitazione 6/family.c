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


typedef char*string;

void printMinus (int n);
int toNumber(string n);
void family (int n);

int main (int argc, string argv[]){

    if(argc != 2){
        fprintf(stderr, "Use %s 'number of process'.", argv[0]);
        return 1;
    }
    
    int n = toNumber(argv[1]);
    printf("%s %d\n", argv[0], n);
    family (n);

    return 0;
}

void  family (int n){
    int pid;
    if((pid =fork())==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(n==0){
        if(pid==0){
            return;
        }
        printf("%d: sono l'ultimo discendente\n",pid);
        printf("%d: terminato con successo\n",pid );
        return;
    }

    if(pid==0){
        family(n-1); 
    }else{
        printMinus (n);
        printf("%d: creo un processo figlio\n", pid);
        if(waitpid(pid,(int*)NULL, 0)==-1){
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        printMinus(n);
        printf("%d: terminato con successo\n", pid);
    }

    
}


void printMinus (int n){
    for(n;n>0;n--){
        printf("-");
    }
    printf(" ");
}

int toNumber(string n){
    string e=NULL;
    int num =strtol(n,&e,0);
    if(e!=NULL &&  *e == (char)0 && num>1){
        return num;
    }
    fprintf(stderr, "%s isn't a valid number", n);
    exit(EXIT_FAILURE);
}