#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


typedef int* fPtr (char*);

typedef struct  f{
    fptr fun;
}funArrayType;

int nPrint (char*arg, char**out);
int mPrint (char*, char**out);
int oPrint (char*, char**out);
int hPrint (char*,char**out);



int main(int argc, char* argv[]) {
    if(argc<2){
        puts("ERRROR:Not enough arguments!!!");
        return 1;
    }
 
    opterr=0;
    int opt =0;
    //stampa del nome del programma
    printf("%s ", argv[0]);
    while ((opt = getopt(argc,argv, "n:m:o:h")) != -1) {
        switch(opt) {
        case '?': 
             printf(" -%c: Not an option ", (char)optopt);
             break;  
        default:
        // invocazione della funzione di gestione passando come parametro l'argomento restituito da getopt
        if (V[opt%4]( (optarg==NULL ? argv[0] : optarg) ) == -1) {
        <gestione errore>
        }
        }
    }
    return 0; 
} 

int hPrint (char* ignored){
    //la funzione che stampa l'help non può avere nuulla di sbagliato
    puts("-n <num. intero> -m <num. intero> -o <stringa> -h");
    return 0;
}

int oPrint (char*arg){
    if(arg==NULL){
        return -1;
    }else{
        printf("-s: %s ", arg);
        return 0;
    }
}

int nPrint (char*arg){
    long int value=0;
    if((value=isNumber(arg))!=-1){
        printf(" -n: %ld ",value);
        return 0;
    }else{
        return -1;
    }
}

int mPrint(char*arg){
   long int value=0;
    if((value=isNumber(arg))!=-1){
        printf("-m: %ld ",value);
        return 0;
    }else{
        return -1;
    }
}

long isNumber(const char* s){
    char* e = NULL;
    long val = strtol(s, &e, 0);
    if (e != NULL && *e == (char)0) return val; 
    return -1;
}
