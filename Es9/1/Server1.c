
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

int calcolatrice (string * input, string * output);

int main (void){
    int fd_socketServer;
    int fd_client;
    
    SocketAddress socket_addr;
    strncpy(socket_addr.sun_path, SOCKNAME, UNIX_PATH_MAX);
    socket_addr.sun_family = AF_UNIX;

    fd_socketServer = socket(AF_UNIX,SOCK_STREAM,0);
    if(fd_socketServer==-1){
        perror("error in server socket()");
        exit(EXIT_FAILURE);
    }
    //setto il riutilizo dell'inirizzo
    if(setsockopt(fd_socketServer, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <0 ){
        perror("impossibile riusare l'address");
        exit(EXIT_FAILURE);
    }

    if(bind(fd_socketServer, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) !=0){
        perror("server bind()");
        close(fd_socketServer);
        exit(EXIT_FAILURE);
    }
    if(listen(fd_socketServer, SOMAXCONN) == -1){
        perror("server listen()");
        exit(EXIT_FAILURE);
    }
    puts("server pronto");
    fd_client = accept(fd_socketServer , NULL , 0);

    string output = NULL;;
    int output_size;
    string buf = NULL; //input

    while (true){
        
        if(buf!=NULL){free(buf);}
        buf = malloc(sizeof(char)*N);
        read(fd_client,buf,N);
        if(strstr(buf, "exit")){
            //se la stringa contiene exit esco
            write(fd_client,"Addio",5);
            close(fd_client);
            close(fd_socketServer);
            return 0;
        }
        //stampo l'operazione richiesta dal client
        fprintf(stdout, "Operazione:%s\n", buf);

        if((output_size = calcolatrice(&buf,&output))<0){
            //se la dimensione dei byte letti è sbagliata stampo che c'è stato un errore
            //comunico al client chye c'è stato un errore, chiudo la socket client
            //chiudo la socket server ed esco;
            fprintf(stdout, "Errore\n");
            write(fd_client, "errore", 7);
            close(fd_client);
            close(fd_socketServer);
            exit(EXIT_FAILURE);
        }
        //qui va scritto l'output
        output[output_size]='\0';
        write(fd_client, output, strlen(output));
        free(output);
    }//end while true

    close(fd_client);
    if(output!=NULL){
        free(output);
        }
    
    if(buf!=NULL){
        free(buf);
    }
    close(fd_socketServer);
    return EXIT_SUCCESS;
}

int calcolatrice (string * input, string * output){
    //odio questa cazzo di bc
    int pipe1[2];
    int pipe2[2];
    pid_t id;

    if(pipe(pipe1)==-1 || pipe(pipe2)==-1){
        perror("pipe()");
        return -1;
    }

    if((id=fork())==-1){
        perror("fork()");
        return -1;
    }
    if(id==0){
        
        close(STDOUT_FILENO);
        close(STDIN_FILENO);
        close(pipe1[0]);
        close(pipe2[1]);
        //stdout al padre, stdio al figlio
        if((dup2(pipe1[1],STDOUT_FILENO) ==-1) || (dup2(pipe2[0],STDIN_FILENO))==-1){
            perror("dup2()");
            return -1;
        }
        int value ;
        value = execl("/usr/bin/bc", "bc -lq", (string)NULL);
        fprintf(stdout, "execl end with value: %d",value);
        exit(value);
    }else{
        close(pipe1[1]);
        close(pipe2[0]);
        *output=malloc(sizeof(char)*N);
        int err;
        err=write(pipe2[1], *input, strlen(*input));
        err=write(pipe2[1],"\n", 1);
        if(err ==-1){
            perror("write()");
            return -1;
        }
        ssize_t dim = read(pipe1[0],*output, N);
        if(dim<0){
            perror("read()");
            return -1;
        } 

        close(pipe1[0]);
        close(pipe2[1]);
        return dim;
    }

}