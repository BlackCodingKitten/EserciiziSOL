
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
#define SOCKNAME "./socket1"


#ifndef N
#define N 100
#endif

typedef char* string;
typedef struct sockaddr_un SocketAddress;


int main (void){
    SocketAddress socket_addr;
    strncpy(socket_addr.sun_path, SOCKNAME,UNIX_PATH_MAX);
    socket_addr.sun_family = AF_UNIX;
    int fd_socket;
    fd_socket = socket(AF_UNIX, SOCK_STREAM,0);
    if(fd_socket==-1){
        perror("client socket()");
        exit(EXIT_FAILURE);
    }
    string buffer ;

    while (connect(fd_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) == -1){
        puts("aspetto connessione");
        if(errno == ENOENT){
            sleep(1);
        }else{
            exit(EXIT_FAILURE);
        }
    }
    puts("connesso");
    while(true){
        buffer = malloc(sizeof(char)*N);
        printf("Operazione: ");
        scanf("%s", buffer);
        size_t dim = strlen(buffer)+1;
        write(fd_socket,buffer,dim);
        if(strstr(buffer,"exit")){
            memset(buffer,'\0',N);
            read(fd_socket,buffer,N);
            break;
        }
        memset(buffer,'\0',N);
        read(fd_socket,buffer,N);
        fprintf(stdout, "Risposta: %s",buffer);
        memset(buffer,'\0',N);
        free(buffer);
    }

    

    if(buffer!=NULL){free(buffer);}
    close(fd_socket);
    return EXIT_SUCCESS;   
}