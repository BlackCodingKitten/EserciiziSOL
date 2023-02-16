
#define _POSIX_C_SOURCE 200112L
#define _OPEN_SYS_ITOA_EXT
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>

#define BUFFER_SIZE 100
#define UNIX_PATH_MAX 108
#define SOCKET_NAME "./MWSocket"


typedef char* string;
bool recivedSig = false;
static int N;

typedef struct workerArgs{
    int workerIndex;
    int clientSocket;
}WArgs;

typedef struct thread{
    pthread_t id;
    bool busy;
}Thread;
Thread* WArray;
WArgs* WArgsArray;

string strtoUp (string s);
void* manager (void*arg);
void* worker(void*arg);
void* sigHandler(void* arg);

int listenSocket;




int main (int argc, string argv[]){
    if(argc!=2){
        fprintf(stderr, "Usage %s N\nN èun intero positivo.", argv[0]);
        exit(1);
    }else{
        string e=NULL;
        N = strtol(argv[1],&e,0);
        if(e==NULL || *e!=(char)0){
            fprintf(stderr, "Usage %s N\nN èun intero positivo.", argv[0]);
            exit(1);
        }
    }

    //getsione segnali
    pthread_t sigHandlerThreadId;
    sigset_t mask;
    int err=0;
    if(sigemptyset(&mask)==-1){
        perror("sigemptset()");
        exit(EXIT_FAILURE);
    }
    err=sigaddset(&mask, SIGINT);
    err=sigaddset(&mask, SIGQUIT);
    if(err!=0){
        write(STDERR_FILENO, "sigaddset()\n", strlen("sigaddset()\n")+1);
        exit(EXIT_FAILURE);
    }
    if(pthread_sigmask(SIG_BLOCK,&mask,NULL)==-1){
        perror("sigmask()");
        exit(1);
    }
    if(pthread_create(&sigHandlerThreadId, NULL, &sigHandler, (void*)&mask)==-1){
        perror("create(sighandlerThread)");
        exit(EXIT_FAILURE);
    }
    //ignorare SIGPIPE
    struct sigaction sigpipe;
    memset(&sigpipe, 0, sizeof(sigpipe));
    sigpipe.sa_handler=SIG_IGN;
    if(sigaction(SIGPIPE,&sigpipe,NULL)==-1){
        perror("sigaction(SIGPIPE)");
        exit(EXIT_FAILURE);
    }
    //creo la socket del server su cui ascoltare le connessioni
    struct sockaddr_un address;
    strncpy(address.sun_path, SOCKET_NAME, UNIX_PATH_MAX);
    address.sun_family=AF_UNIX;
    listenSocket=socket(AF_UNIX,SOCK_STREAM,0);
    //controllo la socket()
    if(listenSocket==-1){
        perror("socket()");
        return EXIT_FAILURE;
    }
    //eseguo e controllo bind della socket all'indirizzo
    if(bind(listenSocket,(struct sockaddr*)&address,sizeof(address))==-1){
        perror("bind()");
        exit(1);
    }
    //mi metto in ascolto e controllo che l'ascolto avvenga in maniera corretta
    if(listen(listenSocket,SOMAXCONN)==-1){
        perror("listen()");
        return EXIT_FAILURE;
    }

    //creazione manager della pool delle connessioni
    pthread_t managerId;
    if(pthread_create(&managerId,NULL,&manager, NULL)==-1){
        perror("manager thread create()");
        exit(EXIT_FAILURE);
    }
    pthread_join(managerId,NULL);
    return EXIT_SUCCESS;
}


void*sigHandler(void*arg){
    sigset_t*mask = (sigset_t*)arg;
    int sig;
    if(pthread_detach(pthread_self())==-1){
        perror("detach(self())");
        _exit(1);
    }
    while (true){
        if(sigwait(mask, &sig)!=0){
            perror("sigwait()");
            _exit(1);
        }
    fprintf(stdout, "SEGNALE: %d\n", sig);
        //setto la flag di ricezione segnale
        recivedSig=true;
        //rimuovo la socket
        shutdown(listenSocket, SHUT_RDWR);
        execl("/bin/rm","rm -r ",SOCKET_NAME,NULL);
        write(STDOUT_FILENO, "SocketFile removed\n", strlen("SocketFile removed\n")+1);
        return NULL;
    }
    //mai eseguita in teoria 
    _exit(1);
}

void* manager(void*arg){
    WArgsArray=malloc(sizeof(WArgs)*N);
    WArray=malloc(sizeof(Thread)*N);
    //inizializzato aliberi tutti i worker
    for(int i=0; i<N; i++){
        WArray[i].busy=false;
    }

    int index=0;
    int newClient = 0;

    while(true){
        //accetto la connessione del client;
        newClient=accept(listenSocket,NULL,0);
        //cerco il primo worker non impegnato già in un altra connsssione; 
        while(WArray[index].busy){
            index=((index+1)%N);
        }
        //setto gli argomenti: il numero del client e l'indice del thread;
        WArgsArray[index].clientSocket=newClient;
        WArgsArray[index].workerIndex=index;
        //creo il nuovo thread che gestisce la connessione;
        if(pthread_create(&WArray[index].id,NULL,&worker,(void*)&WArgsArray[index])){
            perror("create(workers)");
            exit(1);
        }
        //il Worker[index] è impegnato
        WArray[index].busy=true;
        //risetto index a 0
        index=0;
    }
    //libero la memoria 
    free(WArgsArray);
    free(WArray);
    //esco dal Thread
    pthread_exit(NULL);
}

void* worker (void*arg){
    puts("NuovoClient");
    WArgs data = *((WArgs*)arg);
    if(pthread_detach(pthread_self())==-1){
        perror("detach(self())");
        exit(1);
    }
    while(!recivedSig){
        //messaggio di conferma al client 
        write(data.clientSocket,"Connesso",strlen("Connesso")+1);
        string buffer = malloc(sizeof(char)*BUFFER_SIZE);
        read(data.clientSocket, buffer, BUFFER_SIZE);
        if(strstr(buffer,"exit")){
            write(data.clientSocket,"Arrivederci", strlen("Arrivederci")+1);
            free(buffer);
            close(data.clientSocket);
            //setto di nuovo la connssione libera
            WArray[data.workerIndex].busy=false;
            break;
        }
        write(data.clientSocket, strtoUp(buffer), strlen(buffer)+1);
        free(buffer);
    }
    return NULL; 
}

string strtoUp (string s){
    //sistemata la funzione che traduce in maiuscole;
    for(int i=0; i<strlen(s); i++){
        //printf("DEBUG: %c", s[i]);
         s[i]=toupper(s[i]);
    }
    return s;
}