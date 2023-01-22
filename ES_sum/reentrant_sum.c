#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sum.h"

#ifndef INIT_VALUE
#define INIT_VALUE 0
#endif


int main(int argc, char**argv){
    if(argc < 2 ){
        puts("Please define how many numbers you wanna add");
        return 1;
    }
    char* e = NULL;
    int N = strtol(argv[1], &e, 0);

    int * pointer=NULL;
    int x=0;
    for(int i=1; i<N; i++){      
        scanf("%d", &x);
        if(i==1){
            x+=INIT_VALUE;
        }
        printf("totale parziale: %d\n", sum_r(x,&pointer));
    }
        scanf("%d", &x);
        printf("totale finale: %d\n", sum_r(x,&pointer));   
   return 0; 
}