
#define _POSIX_C_SOURCE 200112L
#define _OPEN_SYS_ITOA_EXT
#include <pthread.h>
#include <unistd.h>
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
#define BUFFER_SIZE 150
typedef char* string;
int main (int argc, string argv[]){
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
        puts("Client in attesa di connessione ");
        if(errno==ENOENT){
            sleep(1);
        }else{
            exit(EXIT_FAILURE);
        }
    }
    string buffer;
    puts("client connesso");
    buffer=malloc (sizeof(char)*BUFFER_SIZE);
    read(clientStream,buffer,BUFFER_SIZE);
    printf("Messaggio dal Server: %s\n", buffer);
    free(buffer);

    while(true){
        buffer=malloc (sizeof(char)*BUFFER_SIZE);
        printf("Stringa da inviare: ");
        scanf("%s",buffer);
        size_t dim =strlen(buffer)+1;
        write(clientStream,buffer,dim);
        if(strstr(buffer,"exit")){
            memset(buffer,'\0', BUFFER_SIZE);
            read(clientStream,buffer,BUFFER_SIZE);
            break;
        }
        memset(buffer,'\0', BUFFER_SIZE);
        read(clientStream,buffer,BUFFER_SIZE);
        fprintf(stdout, "\nRisposta del Server: %s\n", buffer);
        memset(buffer,'\0',BUFFER_SIZE);
        free(buffer);
    }
    if(buffer!=NULL){
        free(buffer);
    }
    close(clientStream);
    return 0;
}

