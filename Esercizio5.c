#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


#define error 1;

void evalueOption(int cIndex,char* v[]);
void addMemory(char**pointer, int plus);

void nPrint (char*);
void mPrint (char*);
void oPrint (char*);
void errPrint (char);

long isNumber(const char* s);

int main (int argc, char* argv[]){
    if(argc==1){
        puts("Not enough arguments");
        return error;
    }
    //controllo se c'è -h
    for(int y=0; y<argc; y++){
        if(strstr(argv[y], "-h")){
            printf("%s:\t -n <num. intero> -m <num. intero> -o <stringa> -h", argv[0]);
            return 0;
        }
    }

    evalueOption(argc,argv);
    return 0;
}

void evalueOption(int cIndex, char* v[]){
    //inserisco il nome del programma


    int opt;
    char*arg=NULL;
    opterr=0;
    printf("%s\t", v[0]);
    while((opt = getopt(cIndex, v, "n:m:o:h:"))!=-1){
        switch (opt){
            case 'n':
                nPrint(optarg);
                break;
            case 'm':
                mPrint(optarg);
                break;
            case 'o':
                oPrint(optarg);
                break;
            case '?':
                errPrint((char)optopt);
                break;
            default:
                abort();
                break;

        }//end switch
    }//end while 
    printf("\n");
}

void errPrint (char c){
    if(c=='o' || c=='n' || c=='m'){
        printf(" -%c: missing arguments ", (char)c);
        return ;
    }
    printf(" -%c: Not an option ", (char)c);
}




void oPrint (char*arg){
    if(arg==NULL){
        printf(" -s: NULL ");
    }else{
        printf("-s: %s ", arg);
    }
}

void nPrint (char*arg){
    long int value=0;
    if((value=isNumber(arg))!=-1){
        printf(" -n: %ld ",value);
    }else{
        printf(" -n: NaN ");
    }
}

void mPrint(char*arg){
   long int value=0;
    if((value=isNumber(arg))!=-1){
        printf("-m: %ld ",value);
    }else{
        printf("-m: NaN ");
    }
}

long isNumber(const char* s){
    char* e = NULL;
    long val = strtol(s, &e, 0);
    if (e != NULL && *e == (char)0) return val; 
    return -1;
}