
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
#define UNIX_PATH_MAX 108 
#define SOCKNAME "./socket2"
#define MAX_CLIENT 100
#define BUFFER_SIZE 150

typedef char* string;
int serverSocket;

string strtoUp (string s);
void * dispatcher (void*arg);
void * worker (void*arg);

int main (void){
    //creiamo la socket server
    struct sockaddr_un serveraddr;
    strncpy(serveraddr.sun_path,SOCKNAME,UNIX_PATH_MAX);
    serveraddr.sun_family=AF_UNIX;

    serverSocket=socket(AF_UNIX, SOCK_STREAM,0);
    if(serverSocket==-1){
        perror("socket()");
        return EXIT_FAILURE;
    }
    //setto il riuso dell'indirizzo
    if(bind(serverSocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==-1){
        perror("bind()");
        return EXIT_FAILURE;
    }

    if(listen(serverSocket,SOMAXCONN)==-1){
        perror("listen()");
        return EXIT_FAILURE;
    }
    pthread_t dispatcherId;
    if(pthread_create(&dispatcherId,NULL,&dispatcher,NULL)!=0){
        perror("dispatcher create()");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }
    pthread_join(dispatcherId,NULL);
    close(serverSocket);
    return 0;

}

void*dispatcher(void * arg){
    int clientSocket[MAX_CLIENT];
    
    pthread_t workerIdArray[MAX_CLIENT];
    for(int i=0; i<MAX_CLIENT; i++){
        clientSocket[i]=accept(serverSocket,NULL,0);
        //creo il thread che gestisce il client;
        if(pthread_create(&workerIdArray[i],NULL,&worker,(void*)&clientSocket[i])==-1){
            perror("pthread_create(WORKER)");
            exit(EXIT_FAILURE);
        }
    }

    pthread_exit(NULL);
}

void* worker(void*arg){
    int client =*((int*)arg);
    printf("DEBUG: client %d\n", client);
    write(client, "Benvenuto", 10);
    string buf= malloc(sizeof(char)*BUFFER_SIZE);
    string out=malloc(sizeof(char)*BUFFER_SIZE);

    while(true){
        memset(buf,'\0', BUFFER_SIZE);
        read(client,buf,BUFFER_SIZE);
        if(strstr(buf, "exit")){
            write(client, "Disconnesso",  12);
            close(client);
            free(buf);
            pthread_exit(NULL);
        }
        fprintf(stdout, "Stringa ricevuta: %s\n", buf);
        out = strtoUp(buf);
        out[strlen(out)]='\0';
        if(out==NULL){
            write(client,"Errore",7);
            //free(buf);
            close(client);
            pthread_exit(NULL);
        }
        write(client, out, strlen(out)+1);
        memset(out, '\0', BUFFER_SIZE);
        free(out);
    }

}

string strtoUp (string s){
    //sistemata la funzione che traduce in maiuscole;
    for(int i=0; i<strlen(s); i++){
        //printf("DEBUG: %c", s[i]);
         s[i]=toupper(s[i]);
    }
    return s;
}