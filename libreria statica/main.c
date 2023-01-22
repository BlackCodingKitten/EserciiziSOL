#include <stdio.h>
#include "prova.h"
#include <stdlib.h>
#include <string.h>

int main (int argc, char** argv){
    char* e = NULL;
    int a = strtol(argv[1], &e, 0);
    e=NULL;
    int b = strtol(argv[2], &e, 0);


    printf("SOMMA: %d\nDIFF: %d\nMULT: %d\n", somma(a,b), diff(a,b), times(a,b));
    return 0;
}