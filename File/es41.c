#define LIMIT 1000
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef char* string;


int main (int argc, string*argv){
    //passo ad argomento il percorso del file su cui scrivere
    if(argc <2){
        puts("Missing Output-file name.");
        return -1;
    }

    string s = malloc(sizeof(char)*LIMIT);

    FILE *inputPtr;
    FILE *outputPtr;

    inputPtr = fopen("/etc/passwd", "r");
    if(inputPtr==NULL){
        perror("Aprendo passwd");
        return -1;
    }

    outputPtr = fopen(argv[1], "w");
        if(outputPtr==NULL){
        perror("Creando l'output ");
        return -1;
    }

    while(fgets(s,LIMIT,inputPtr)!=NULL){
        // DEBUG: puts(s);
        const char* k=":";
        string temp=malloc(sizeof(char)*LIMIT);
        fprintf(outputPtr, "%s\n", strncpy(temp, s,(int)strcspn(s,k)));
    }

    fclose(inputPtr);
    fclose(outputPtr);
    free(s);
    return 0;
}