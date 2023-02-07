#define _POSIX_C_SOURCE 200112L
#define _OPEN_SYS_ITOA_EXT
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#define LIMIT 2000
#define DIM 15
#define SPECIAL "chiusura"
typedef char * string;

/*********QUEUE****FUNCTIONS***********/
struct node{
    string data;
    struct node * nt;
};

typedef struct queue{
    size_t data_size;
    struct node* first;
    struct node* last;
}Queue;

typedef struct b{
    Queue **q;
    bool letto;
}Buffer;

void free_queue (struct queue* q);
string pop (struct queue* q);
struct queue* init_queue (size_t d);
void push (struct queue** qptr, string data);
bool is_empty (struct queue * q);
/************************************/

static int totalP;
int messagePerProducer;
Buffer buff[DIM];
pthread_mutex_t  mutex[DIM+1];
pthread_cond_t  c[DIM];


string message (void);
int toNumber (string n);
void* producerTask (void*arg);
void* consumerTask (void*arg);


int main(int argc,  string argv[]){
    if(argc != 4){
        fprintf(stderr, "Usage: %s M N K\n", argv[0]);
	    return 1;
    }
    
    int arg[3];
    for(int i=0; i<3; i++){
        arg[i]=toNumber(argv[i+1]);
        //printf("%d\n", arg[i]);
    }
    totalP=arg[0];
    messagePerProducer=(int)(arg[2]/arg[0]);

    Queue * q0 = init_queue(sizeof(char)*LIMIT);
    buff[0].q = &q0;
    Queue * q1 = init_queue(sizeof(char)*LIMIT);
    buff[1].q = &q1;
    Queue * q2 = init_queue(sizeof(char)*LIMIT);
    buff[2].q = &q2;
    Queue * q3 = init_queue(sizeof(char)*LIMIT);
    buff[3].q = &q3;
    Queue * q4 = init_queue(sizeof(char)*LIMIT);
    buff[4].q = &q4;
    Queue * q5 = init_queue(sizeof(char)*LIMIT);
    buff[5].q = &q5;
    Queue * q6 = init_queue(sizeof(char)*LIMIT);
    buff[6].q = &q6;
    Queue * q7 = init_queue(sizeof(char)*LIMIT);
    buff[7].q = &q7;
    Queue * q8 = init_queue(sizeof(char)*LIMIT);
    buff[8].q = &q8;
    Queue * q9= init_queue(sizeof(char)*LIMIT);
    buff[9].q = &q9;
    Queue * q10= init_queue(sizeof(char)*LIMIT);
    buff[10].q = &q10;
    Queue * q11= init_queue(sizeof(char)*LIMIT);
    buff[11].q = &q11;
    Queue * q12= init_queue(sizeof(char)*LIMIT);
    buff[12].q = &q12;
    Queue * q13= init_queue(sizeof(char)*LIMIT);
    buff[13].q = &q13;
    Queue * q14 = init_queue(sizeof(char)*LIMIT);
    buff[14].q = &q14;

    for(int i=0 ; i<DIM; i++){
        buff[i].letto=true;
        //gia che ce sto inizializzo anche mutex e cv :) sono pigra
        pthread_mutex_init(&mutex[i],NULL);
        pthread_cond_init(&c[i], NULL);
    }
    pthread_mutex_init(&mutex[DIM],NULL);

    //creo i threads
    pthread_t producerId[arg[0]];
    pthread_t consumerId[arg[1]];
    for (int i=0; i<arg[0]; i++){
        pthread_create(&producerId[i], NULL, &producerTask, NULL);
    }

    for(int i=0; i<arg[1]; i++){
        pthread_create(&consumerId[i], NULL ,&consumerTask,NULL);
    }

    //aspetto la terminazione di tutti i consumer per terminare 
    for(int i=0 ; i<arg[1]; i++){
        pthread_join(consumerId[i], NULL);
    }
    //distruggo mutex e condizioni e libero la memoria
    for(int i=0 ; i<arg[1]; i++){
        pthread_mutex_destroy(&mutex[i]);
        pthread_cond_destroy(&c[i]);
    }
    pthread_mutex_destroy(&mutex[DIM]);
    for(int i=0; i<DIM; i++){
        free_queue(*(buff[i].q));
    }

    return 0;
}

string message(void ){
    string ret = malloc(10*sizeof(char));
    srand(time(0)*rand());
    int r = rand();
    sprintf(ret, "%d", r);
    return ret;
}

int toNumber(string n){
    string e=NULL;
    int m =strtol(n, &e,0);
    if(e !=NULL && *e == (char)0 ){
        return m;
    }
    fprintf(stderr, "Invalid Number %s\n", n);
    exit(EXIT_FAILURE);
}
/****************THREADS**TASK******************/
void * producerTask (void* arg){

    for(int i=0; i<messagePerProducer; i++){
        int index= i % DIM;
        if(pthread_mutex_lock(&mutex[index])==0){
            while (!buff[index].letto){
                pthread_cond_wait(&c[index],&mutex[index]);
            }
            string m = malloc(10*sizeof(char)); 
            strcpy(m,message());
            push(buff[index].q,m);
            free(m);
            buff[index].letto=false;
            pthread_cond_signal(&c[index]);
            pthread_mutex_unlock(&mutex[index]);
        }
    }
    if(pthread_mutex_lock(&mutex[DIM])==0){
        totalP=totalP-1;
        if(totalP<=0){
            int tmp=0;
            while(true){
                int i=tmp%DIM;
                if(pthread_mutex_lock(&mutex[i])==0){
                    while(!buff[i].letto){
                        pthread_cond_wait(&c[i],&mutex[i]);
                    }
                    push(buff[i].q,SPECIAL);
                    buff[i].letto=false;
                    pthread_cond_signal(&c[i]);
                    pthread_mutex_unlock(&mutex[i]);
                }
                tmp++;
            }
        }
        pthread_mutex_unlock(&mutex[DIM]);
    }

}

void* consumerTask (void*arg){

    int i=0;
    int index;
    while(true){
        index = i % DIM;
        i++;
        if(pthread_mutex_lock(&mutex[index])==0){
            while(buff[index].letto){
                pthread_cond_wait(&c[index], &mutex[index]);
            }

            string s=pop(*(buff[index].q));
            if(s==NULL){
                buff[index].letto=true;
                pthread_cond_signal(&c[index]);
                pthread_mutex_unlock(&mutex[index]);
            }else{
                fprintf(stdout, "%s\n", s);
                fflush(stdout);
                buff[index].letto=true;
                pthread_cond_signal(&c[index]);
                pthread_mutex_unlock(&mutex[index]);
                if(strstr(s,SPECIAL)!=NULL){
                    break;
                }
            }

        }
    }
    return NULL;
    
}



/*********QUEUE***FUNCTIONS***********/
void free_queue (struct queue* q){
    if(!q){
        return;
    }
    while (q->first->nt!=NULL){
        struct node * tmp = q->first->nt;
        q->first->nt = (q->first->nt)->nt;
        free(tmp->data);
        free(tmp);
    }
    free(q->first);
    free(q);
}

void push (struct queue** qptr, string data){
    struct node * nodo;
    if((nodo = (struct node* )malloc(sizeof(struct node)))==NULL) {
        perror("malloc nella push");
        exit(EXIT_FAILURE);
    }
    if((nodo->data = (string)malloc((*qptr)->data_size))==NULL){
        perror("malloc nella push");
        exit(EXIT_FAILURE);
    }
    strcpy(nodo->data,data);
    nodo->nt=NULL;
    if(is_empty(*qptr)){
        (*qptr)->first->nt = nodo;
        (*qptr)->last = nodo;
    }else{
        (*qptr)->last->nt = nodo;
        (*qptr)->last = nodo;
    }
}

struct queue* init_queue (size_t d){
    struct node *  head =(struct node*)malloc(sizeof(struct node));
    head ->data=NULL;
    head->nt=NULL;
    struct queue * coda = (struct queue *)malloc(sizeof(struct queue));
    coda->data_size = d;
    coda->first=head;
    coda->last=head;
    return coda;
}

string pop (struct queue * q){
    if(is_empty(q)){
        return NULL;
    }
    struct node * tmp = (q)->first->nt;
    if(tmp->nt != NULL){
        (q)->first->nt = tmp->nt;
    }else{
        (q)->first->nt =NULL;
        (q)->last = (q)->first;
    }
    string ret = (string) malloc(sizeof(char)*(strlen(tmp->data)+1));
    strcpy(ret,tmp->data);
    free(tmp->data);
    free(tmp);

    return ret;
}

bool is_empty (struct queue * q){
    return q->first == q->last;
}