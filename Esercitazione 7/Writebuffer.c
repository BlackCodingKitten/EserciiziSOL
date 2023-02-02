#define _POSIX_C_SOURCE 200112L
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#ifndef N_OF_N
#define N_OF_N 10
#endif 

typedef char* string;

typedef struct b{
    int v;
    bool empty;
}buffer;

buffer a;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t c = PTHREAD_COND_INITIALIZER;


void riempiBuffer (int n);
int svuotaBuffer (void);
int randomNumber (void);
void* producerTask (void* arg);
void* consumerTask (void* arg);

int main (void){
    a.v=0;
    a.empty=true;
    //CREO I THREAD
    pthread_t Producer_id;
    pthread_t Consumer_id;
    if(pthread_create(&Producer_id,NULL,&producerTask,NULL)!=0){
        perror("creazione thread producer");
        return EXIT_FAILURE;
    }
    if(pthread_create(&Consumer_id, NULL, &consumerTask,NULL)!=0){
        perror("creazione thread consumer");
        return EXIT_FAILURE;
    }

    pthread_join(Producer_id,NULL);
    pthread_join(Consumer_id,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&c);
    return EXIT_SUCCESS;
}

void riempiBuffer (int n){
    a.v=randomNumber();
    //printf(": %d\n", a.v);
    fflush(stdout);
    a.empty=false;
}

int svuotaBuffer (void){
    a.empty=true;
    return a.v;
}

int randomNumber(void){
    srand(time(0)*rand());
    return (int)rand();
}

void* producerTask(void*arg){
    int p=0;
    while (p< N_OF_N){
        if(pthread_mutex_lock(&mutex)==0){
            if(!a.empty){
              //  fprintf(stdout, "--Thread Producer in attesa che sia svuotato il buffer\n");
                fflush(stdout);
                pthread_cond_wait(&c,&mutex);//il mutex viene liberato
            }
           // fprintf(stdout, "--Thread Producer sta rimepiendo il buffer");
            fflush(stdout);
            riempiBuffer(randomNumber());
            pthread_cond_signal(&c);
            p++;
            pthread_mutex_unlock(&mutex);
        }else{
           // fprintf(stdout, "--Thread Producer non ha acquisito la lock\n");
           // fflush(stdout);
        }
    }

}
void* consumerTask (void*arg){
    int ct = 0;
    while(ct<N_OF_N){
        if(pthread_mutex_trylock(&mutex)==0){
            if(a.empty){
                //fprintf(stdout,"Thread Consumer in attesa che il buffer sia riempito\n");
                fflush(stdout);
                pthread_cond_wait(&c,&mutex);
            }
            fprintf(stdout,"Thread Consumer svuota il buffer: %d\n", svuotaBuffer());
            fflush(stdout);
            pthread_cond_signal(&c);
            ct++;
            pthread_mutex_unlock(&mutex);
        }else{
            //fprintf(stdout, "Thread Consumer non ha acquisito la lock\n");
            //fflush(stdout);
        }
    }
}