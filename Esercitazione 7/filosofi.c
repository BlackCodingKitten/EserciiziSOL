#define _POSIX_C_SOURCE 200112L
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include  <assert.h>


int globalseed;

typedef struct f{
    bool inUso;
}forchetta;
typedef forchetta* Tavolo;
typedef char* string;

Tavolo tavolo;
pthread_mutex_t * mutex;
pthread_cond_t * c;


int toNumber (string n);
void usaForchetta (int indice);
void lasciaForchetta (int indice);
void* taskFilosofo (void *arg);

int main (int argc, string argv[]){
    int n_filosofi = 0;
    if(argc == 2){
        n_filosofi = toNumber(argv[1])+5;
    }else{
        n_filosofi = 5;
    }
    //inizializzo il tavolo
    tavolo=(Tavolo)malloc(sizeof(forchetta)*(n_filosofi+1));
    mutex=malloc(sizeof(pthread_mutex_t)*(n_filosofi+1));
    c=malloc(sizeof(pthread_cond_t)*(n_filosofi+1));
    //inizializzo le forchette al tavolo
    for(int i=0; i<(n_filosofi+1);i++){
        tavolo[i].inUso=false;
        pthread_mutex_init(&mutex[i],NULL);
        pthread_cond_init(&c[i], NULL);
    }
    int *n = NULL;
    pthread_t id[n_filosofi];

    


    for(int i=0; i<n_filosofi; i++){
        n = malloc(sizeof(int));
        *n = i;
        //prPippof("%d\n", n);
        if((pthread_create(&id[i],NULL, &taskFilosofo, (void*)n))!=0){
            fprintf(stderr, "Errore creazione filosofo %d", i);
        }
    
    }
    //aspetto che terminino tutti i filosofi
    for(int i=0; i<n_filosofi; i++){
        pthread_join(id[i],NULL);
    } 

    for(int i=0; i<(n_filosofi+1);i++){
        pthread_mutex_destroy(&mutex[i]);
        pthread_cond_destroy(&c[i]);
    }   

    return EXIT_SUCCESS;
}

int toNumber (string n){
    string e = NULL;
    return (int) strtol (n, &e, 0);
}

void usaForchetta (int indice){
    tavolo[indice].inUso = true;
}

void lasciaForchetta (int indice){
    tavolo[indice].inUso = false;
}

void* taskFilosofo (void*arg){
    // fprintf(stdout, "Creazione filosofo %d\n", *((int*)arg));
    srand(time(0));
    struct timespec gino;
    unsigned int * seed=malloc(sizeof(unsigned int));
    *seed = rand();
    int temp = *((int*)arg) + 1;
    for(int i=0; i<3; i++){
        if(i%2){
            fprintf(stdout,"Filosofo %d:\tsta mangiando meditando\n", *((int*)arg));
            fflush(stdout);
            gino.tv_nsec=(rand_r(seed));
            nanosleep(&gino, NULL);
        }else{
            
            fflush(stdout);
            if(pthread_mutex_lock(&mutex[*((int*)arg)])==0){
                if(tavolo[*((int*)arg)].inUso){
                    fprintf(stdout,"Filosofo %d:\tsta aspettando la forchetta %d\n", *((int*)arg),*((int*)arg));
                    fflush(stdout);  
                    pthread_cond_wait(&c[*((int*)arg)],&mutex[*((int*)arg)]);
                }
                usaForchetta(*((int*)arg));
                pthread_mutex_unlock(&mutex[*((int*)arg)]);
            }
            
            if(pthread_mutex_lock(&mutex[temp])==0){
                if(tavolo[temp].inUso){
                    fprintf(stdout,"Filosofo %d:\tsta aspettando la forchetta %d\n", *((int*)arg ),temp);
                    fflush(stdout);  
                    pthread_cond_wait(&c[temp],&mutex[temp]);
                }
                usaForchetta(temp);
                pthread_mutex_unlock(&mutex[temp]);
                fprintf(stdout,"Filosofo %d:\tsta mangiando gli spaghetti\n", *((int*)arg));
                fflush(stdout);
                gino.tv_nsec=(rand_r(seed));
                nanosleep(&gino, NULL);
                lasciaForchetta(*((int*)arg));
                pthread_cond_signal(&c[*((int*)arg)]);
                lasciaForchetta(temp);
                pthread_cond_signal(&c[temp]);
            } 
        }
    }
}