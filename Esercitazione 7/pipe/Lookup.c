#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define LIMIT 1024
bool finefile = false;
bool finetoken =false;

/*********UTILS***********/
typedef char* string;

struct node{
    string data;
    struct node * nt;
};

struct queue{
    size_t data_size;
    struct node* first;
    struct node* last;
};

typedef struct mybuffer{
    struct queue** b;
    bool done; //è false se è appena stato scritto,  è true se è appena stato letto 
}MyBuffer;
MyBuffer bufferT1T2;
MyBuffer bufferT2T3;

/*********MUTEXS******CONDITION*VARIABLES***********/
pthread_mutex_t mutexT1T2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cT1T2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexT2T3= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cT2T3 = PTHREAD_COND_INITIALIZER;

/*********THREADS**TASKS***********/
void* tokenTask (void*arg);
void* readTask (void*arg);
void* printTask(void*arg);

/*********QUEUE****FUNCTIONS***********/
void free_queue (struct queue* q);
string pop (struct queue* q);
struct queue* init_queue (size_t d);
void push (struct queue** qptr, string data);
bool is_empty (struct queue * q);

/********MAIN********/
int main (int argc, string argv[]){
    if(argc!=2){
        fprintf(stderr, "Usage: %s 'filename'\n", argv[0]);
        return EXIT_FAILURE;
    }
    //inizializzo le variabili del buffer
    bufferT1T2.done=true;
    bufferT2T3.done=true;
    //inizializzo le code nei buffer
    struct queue * t1t2 = init_queue(sizeof(char)*(LIMIT));
    struct queue * t2t3 = init_queue(sizeof(char)*(LIMIT));
    bufferT1T2.b = &t1t2;
    bufferT2T3.b = &t2t3;

    //creo i thread e i loro id    
    pthread_t id[3];
    pthread_create(&id[0],NULL,&readTask,(void*)&argv);
    pthread_create(&id[1],NULL,&tokenTask,NULL);
    pthread_create(&id[2],NULL,&printTask,NULL);

    //faccio la join sui thread
    for(int i=0; i<3; i++){
        pthread_join(id[i],NULL);
    }

    //distruggo mutex e condizioni e libero la memoria
    pthread_mutex_destroy(&mutexT1T2);
    pthread_mutex_destroy(&mutexT2T3);
    pthread_cond_destroy(&cT1T2);
    pthread_cond_destroy(&cT2T3);
    free_queue(*bufferT1T2.b);
    free_queue(*bufferT2T3.b);

    return 0;
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
    puts("ENTRO  NELLA PUSH");

    struct node * nodo;
    if((nodo = (struct node* )malloc(sizeof(struct node)))==NULL) {
        perror("malloc nella push");
        exit(EXIT_FAILURE);
    }
    if((nodo->data = (string)malloc((*qptr)->data_size))==NULL){
        perror("malloc nella push");
        exit(EXIT_FAILURE);
    }
    // nodo->data = realloc(nodo->data,sizeof(char)*(strlen(data)+1));
    strcpy(nodo->data,data);
    nodo->nt=NULL;
    if(is_empty(*qptr)){
        (*qptr)->first->nt = nodo;
        (*qptr)->last = nodo;
    }else{
        (*qptr)->last->nt = nodo;
        (*qptr)->last = nodo;
    }
    puts("ESCO DALLA PUSH");
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
    puts("ENTRO NELLA POP");

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
    //memcpy(ret, tmp->data, (*q)->data_size);
    puts("ESCO DALLA POP");
    free(tmp->data);
    free(tmp);
    return ret;
}

bool is_empty (struct queue * q){
    return q->first == q->last;
}
/*********THREADS*******TASKS***********/
void* readTask (void*arg){
    string filepath = (*((string**)arg))[1];
    //printf("DEBUG: %s \n", filepath);fflush(stdout); IL FILE LO APRE RISOLTO
    FILE * f;
    if((f = fopen (filepath, "r"))==NULL){
        perror(filepath);
        exit(EXIT_FAILURE);
    }

    string s = malloc(sizeof(char)*LIMIT);

    while(fgets(s,LIMIT,f)!=NULL){
        //leggo la stringa dal file 
        if(pthread_mutex_lock(&mutexT1T2)==0){
            while(!bufferT1T2.done){
                 printf("DEBUGT1: aspetto signal dal th2\n");fflush(stdout);
                 pthread_cond_wait(&cT1T2, &mutexT1T2);
            }
            //inserisco la tringa ne buffer dei thread T1 T2
            printf("DEBUGT1: th1 scrive %s \n", s);fflush(stdout);   
            push(bufferT1T2.b, s);
            bufferT1T2.done = false;
            printf("DEBUGT1: th1 lancia signal per th2\n");fflush(stdout);
            pthread_cond_signal(&cT1T2);
            pthread_mutex_unlock(&mutexT1T2);
            // free(s);
            // s=malloc(sizeof(char)*LIMIT);
        }
    }
    //finita la lettura del file th1 termina
    free(s);
    fclose(f);
    printf("DEBUG: th1 termina l'esecuzione\n");fflush(stdout);
    finefile=true;
    return NULL;
}

void* tokenTask (void*arg){
    // thread2 deve consumare il prodotto di thread1 e produrre per il thread3
    //FASE 1: consumare il prodotto di th1 
    string appo;
    string tmp_buffer;
    
    string token;

    while(true){

        if(finefile && is_empty(*(bufferT1T2.b))){
            break;
        }

        if(pthread_mutex_lock(&mutexT1T2)==0){
            while(bufferT1T2.done){
                printf("DEBUGT2: aspetto signal dal th1\n");fflush(stdout);
                pthread_cond_wait(&cT1T2,&mutexT1T2);
            }
            appo = pop(*(bufferT1T2.b));
            printf("DEBUGT2: th2 legge %s \n", appo);fflush(stdout);
            bufferT1T2.done=true;
            pthread_cond_signal(&cT1T2);
            pthread_mutex_unlock(&mutexT1T2);

            //FASE 2: produrre per il thread 3
            tmp_buffer= calloc (strlen(appo)+1,sizeof(char));
            memcpy(tmp_buffer,appo,strlen(appo));
            free(appo);
            string tok_checkpoint;
            token = strtok_r(tmp_buffer, " ", &tok_checkpoint);
            while(token!=NULL){
                printf("DEBUGT2: tokenizzazione della stringa %s\n", tmp_buffer);fflush(stdout); 
                if(pthread_mutex_lock(&mutexT2T3)==0){
                    while(!bufferT2T3.done){
                        printf("DEBUGT2: th2 aspetta signal dal th3\n");fflush(stdout); 
                        pthread_cond_wait(&cT2T3,&mutexT2T3);
                    }
                    token[strcspn(token,"\n")]= '\0';
                    printf("DEBUGT2: th2 scrive il token %s su th3\n", token);fflush(stdout);
                    push(bufferT2T3.b, token);
                    bufferT2T3.done=false;
                    printf("DEBUGT2: th2 lancia signal per th3\n");fflush(stdout);
                    pthread_cond_signal(&cT2T3);
                    pthread_mutex_unlock(&mutexT2T3);
                    token  = strtok_r (NULL, " ", &tok_checkpoint);
                }
                printf("DEBUGT2: th2 lancia signal per th1\n");fflush(stdout);              
            }
            free(tmp_buffer);
        }else{
            continue;
        }
        
        // free(appo);
    }
    finetoken = true;
    return NULL;
}

void* printTask(void*arg){
    //thread3 consuma quello che è stato prodotto dal thread2
    while (true){
        if(finetoken && is_empty(*(bufferT1T2.b))){
            break;
        }

        if(pthread_mutex_lock(&mutexT2T3)==0){
            while(bufferT2T3.done){
                printf("DEBUGT3: th3 aspetta segnale da t2\n");fflush(stdout);
                pthread_cond_wait(&cT2T3,&mutexT2T3);
            }
            string tmp = pop(*(bufferT2T3.b));
            fprintf(stdout,"--------------------------------------%s\n", tmp);
            printf("DEBUGT3: th3 lancia segnale a T2\n");fflush(stdout);
            bufferT2T3.done=true;
            pthread_cond_signal(&cT2T3);
            pthread_mutex_unlock(&mutexT2T3);
            free(tmp);
        }
    }
    return NULL;
}
