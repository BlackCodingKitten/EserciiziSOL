#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>




typedef struct  f{
    int (*fun) (char*,char**);
}funArrayType;

int nPrint (char*arg, char**out);
int mPrint (char*arg, char**out);
int oPrint (char*arg, char**out);
int hPrint (char*arg,char**out);

long isNumber(const char* s);
int evalue(char c){
    if(c =='m'){
        return 1;
    }
    if(c =='n'){
        return 0;
    }
    if(c=='h'){
        return 3;
    }
    return 2;
}

int main(int argc, char* argv[]) {
    if(argc<2){
        puts("ERROR:Not enough arguments!!!");
        return 1;
    }
    char*output=malloc(sizeof(char));


    funArrayType V[4];
    V[0].fun=nPrint;
    V[1].fun=mPrint;
    V[2].fun=oPrint;
    V[3].fun=hPrint;
    //stampa del nome del programma
    printf("%s ", argv[0]);

    opterr=0;
    int opt =0;
    while ((opt = getopt(argc,argv, "n:m:o:h")) != -1) {
        switch(opt) {
        case '?': 
             output=realloc(output,strlen(output)+strlen(" invalid option "));
             strcat(output, "invalid option");
             break;  
        default:
        // invocazione della funzione di gestione passando come parametro l'argomento restituito da getopt
       
        if ((V[evalue(opt)].fun)(optarg,&output) == -1) {
            if((char)opt=='o' || (char)opt=='n' || (char)opt=='m'){
                output=realloc(output, sizeof(char)*(strlen(" Invalid argument ")+4+strlen(output)));
                strcat(output, " -");
                char temp = (char)opt;
                strcat(output, &temp);
                strcat(output, ": Invalid argument ");
            }
            if((char)opt=='h'){
                free(output);
                break;//esce dal while
            }
        }
        }
    }
    puts(output);
    return 0; 
} 

int hPrint (char* arg, char**out){
    printf("-n <num. intero> -m <num. intero> -o <stringa> -h");
    return -1;//esce in automatico dal cilco con l'help
}

int oPrint (char*arg, char**out){
    if(arg==NULL){
        return -1;
    }else{
        *out= realloc(*out,(sizeof(char)*(strlen(arg)+5+strlen(*out))));
        strcat(*out, " -o: ");
        strcat(*out,arg);
        return 0;
    }
}

int nPrint (char*arg, char** out){
    long int value=0;
    if((value=isNumber(arg))!=-1){
        *out= realloc(*out,sizeof(char)*(strlen(arg)+5+strlen(*out)));
        strcat(*out," -n: ");
        strcat(*out,arg);
        return 0;
    }else{
        return -1;
    }
}

int mPrint(char*arg, char** out){
    long int value=0;
    if((value=isNumber(arg))!=-1){
        *out=realloc(*out,sizeof(char)*(strlen(arg)+5+strlen(*out)));
        strcat(*out," -n: ");
        strcat(*out,arg);
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
