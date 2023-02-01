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
/* 
 * Calcola ricorsivamente il numero di Fibonacci dell'argomento 'n'.
 * La soluzione deve effettuare fork di processi con il vincolo che 
 * ogni processo esegua 'doFib' al più una volta.  
 * Se l'argomento doPrint e' 1 allora la funzione stampa il numero calcolato 
 * prima di passarlo al processo padre. 
 */
static void doFib(int n, int doPrint);
int fibonacci(int n);

int main(int argc, char *argv[]) {
    // questo programma puo' calcolare i numeri di Fibonacci solo fino a 13.  
    const int NMAX=13;

    
    if(argc != 2){
        fprintf(stderr, "Usage: %s <num>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* e=NULL;
    int arg=strtol(argv[1], &e, 0);

    if(e==NULL|| *e != (char)0  || arg <= 0 || arg > NMAX){
        fprintf(stderr, "num deve essere compreso tra 1 e 13\n");
        return EXIT_FAILURE;
    }   

    doFib(arg, 1);
    return 0;
}

static void doFib(int n, int doPrint){
    int pid;
    if((pid=fork())==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(n==0){
        if(pid==0){
            return;
        }
        if(doPrint==1){
            fprintf(stdout, "Processo %d: valore %d\n", pid,fibonacci(n));
            //calcola fibonacci
            return;
        }
    }
    if(pid==0){
        doFib(n-1,doPrint);
    }else{
        //calcola fibonacci
        if(waitpid(pid, (int*)NULL,0)==-1){
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if(doPrint==1){
            fprintf(stdout,"Processo %d: valore %d\n", pid,fibonacci(n));
        }
    }
}

int fibonacci(int n){
    if(n==0 || n==1){
        return 1;
    }else{
        return fibonacci(n-1)+fibonacci(n-2);
    }
}