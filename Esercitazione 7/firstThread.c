#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>


typedef char* string;

static int global;
pthread_mutex_t mutex =PTHREAD_MUTEX_INITIALIZER;


static void* myfun1 (void*arg);
static void* myfun2 (void*arg);
bool condizione(void){
    if(global <20){
        return true;
    }else{
        return false;
    }
}

int main (void){
    pthread_t p_id;
    pthread_t c_id;
   // pthread_mutex_init(&mutex,NULL);

    pthread_create(&p_id,NULL,&myfun1,NULL);
    pthread_create(&c_id,NULL,&myfun2,NULL);
    //la join serve per aspettare la terminazione dei thread
    pthread_join(p_id,NULL);
    pthread_join(c_id,NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}

static void* myfun1 (void*arg){

    while(condizione()){
        if(pthread_mutex_trylock(&mutex) == 0){
            printf("Thread p: %d\n", ++global);
            fflush(stdout);
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }else{
            printf("no lock in p\n");
            continue;
        }
    }
  
}

static void* myfun2 (void*arg){

    while(condizione()){
        if(pthread_mutex_trylock(&mutex)==0){
            printf("Thread c: %d\n", ++global);
            fflush(stdout);
            pthread_mutex_unlock(&mutex);
            sleep(1);
        }else{
            printf("no lock in c\n");
            continue;
        }

    }

}