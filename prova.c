
#include <stdio.h>
#include <stdlib.h>

int somma (int a, int b){ return a+b;}
int diff  (int a, int b){return a-b;}
int times (int a, int b){return a*b;}
int max (int a, int b){
    if(a>b){
        return a;
    }
    return b;
}



typedef struct f{
    int (*fPtr) (int,int);
}funPtr;

int main (int argc, char**argv){
    char* e = NULL;
    int val = strtol(argv[1], &e, 0);
    e=NULL;
    int val2 = strtol(argv[2], &e, 0);

    funPtr V[4];
    V[0].fPtr=somma;
    V[1].fPtr=diff;
    V[2].fPtr=times;
    V[3].fPtr=max;


     
    for(int i=0;  i<4; i++){
        int temp=(V[i].fPtr)(val,val2);
        printf("%d\n", temp);
    }

    
    
    
    return 0;
}