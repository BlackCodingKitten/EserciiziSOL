#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



typedef  struct{
    int v;
    int c;
}e;

#define N 1000;
#define K1 100;
#define K2 120;

void inizializzaArray(e*a, int k);
void aggiornaArray(e*a, int n,int k1, int k2);


int main(int argc, char *argv[]){
   
    int n=0;
    int k1=0;
    int k2=0;
    if(argc==4){ 
        char* e = NULL;
        n = strtol(argv[1], &e, 0);
        e=NULL;
        k1= strtol(argv[2], &e, 0);
        e=NULL;
        k2= strtol(argv[3], &e, 0); 
    }else{
        // non sono stati passati i valori di  N K1 K2  da input uso quindi quelli di defult 
        n=N;    k1=K1;  k2=K2;
    }
    //inizializzo l'array
    e*array=malloc(sizeof(e)*k2);
    inizializzaArray(array,k2);
    aggiornaArray(array,n,k1,k2);


    //stampa
    for (int i=0; i<k2; i++){
        printf("%d   :  %.2f%%\n", array[i].v, ((float)array[i].c/n)*100);
    }
    return 0;
}

void inizializzaArray(e*array,int k){   
    for(int j=0; j<k; j++){
        array[j].v=j;
        array[j].c=0;
    }

}



void aggiornaArray(e*a, int n, int k1, int k2){
    for(int i=0; i<n; i++){
        int temp=((rand()%k2)+k1);
        for (int x=0; x<k2; x++){
            if(a[x].v==temp){
                a[x].c++;
            }
        }
    }
}