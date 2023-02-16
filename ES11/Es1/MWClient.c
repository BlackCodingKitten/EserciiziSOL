
#define _POSIX_C_SOURCE 200112L
#define _OPEN_SYS_ITOA_EXT
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <pthread.h>
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
#define SOCKNAME "./MWSocket"

typedef char* string;
void* runClient (void*arg);


int main (void){
    pthread_t id;
    for(int i=0; i<100; i++){
        pthread_create(&id,NULL,&runClient, NULL);
    }
    pthread_join(id,NULL);
    return 0;
}

void* runClient (void*arg){
    struct sockaddr_un addr;
    strncpy(addr.sun_path, SOCKNAME, UNIX_PATH_MAX);
    addr.sun_family=AF_UNIX;
    int clientStream;
    clientStream=socket(AF_UNIX,SOCK_STREAM, 0);
    if(clientStream==-1){
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    while(connect(clientStream,(struct sockaddr*)&addr, sizeof(addr))==-1){
        puts("Client in attesa di connessione");
        if(errno==ENOENT){
            sleep(1);
        }else{
            exit(EXIT_FAILURE);
        }
    }
    string buffer= malloc(sizeof(char)* BUFFER_SIZE);
    read(clientStream, buffer, BUFFER_SIZE);
    fprintf(stdout, "%s\n", buffer);
    fflush(stdout);
    memset(buffer,'\0', BUFFER_SIZE);
    for(int i=0; i<3;i++){
        strcat(buffer,"gatto");
        write(clientStream, buffer, strlen(buffer)+1);
        read(clientStream,buffer, BUFFER_SIZE);
        fprintf(stdout, "%s", buffer);
        fflush(stdout);
    }
    memset(buffer, '\0', BUFFER_SIZE);
    write(clientStream, "exit", strlen("exit")+1);
    read(clientStream, buffer, BUFFER_SIZE);
    fprintf(stdout, "%s\n", buffer);
    fflush(stdout);
    free(buffer);
    pthread_exit(NULL);    
}
