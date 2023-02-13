
#define _POSIX_C_SOURCE 200112L
#define _OPEN_SYS_ITOA_EXT
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/un.h>
#define UNIX_PATH_MAX 108
#define SOCKNAME "./socket3"
#define MAX_CLIENT 100
#define BUFFER_SIZE 150

typedef char *string;

string strtoUp(string s);


int main(void)
{
    struct sockaddr_un sAddr;
    strncpy(sAddr.sun_path, SOCKNAME, UNIX_PATH_MAX);
    sAddr.sun_family = AF_UNIX;
    
    //creo la socket
    int serverSocket;
    if ((serverSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket()");
        return EXIT_FAILURE;
    }
    if(bind(serverSocket,(struct sockaddr*)&sAddr,sizeof(sAddr))==-1){
        perror("bind()");
        return EXIT_FAILURE;
    }

    if(listen(serverSocket,SOMAXCONN)==-1){
        perror("listen()");
        return EXIT_FAILURE;
    }
    
    //inizializzare il set delle socket attive:
    fd_set activeSet;
    fd_set readSet;
    
    FD_ZERO(&activeSet);
    FD_SET(serverSocket,&activeSet);

    int i;

    while (true){
        readSet=activeSet;
        if(select(FD_SETSIZE,&readSet,NULL,NULL,NULL)<0){
            perror("select()");
            close(serverSocket);
            execl("/bin/rm","rm -r",SOCKNAME,NULL);
            return EXIT_FAILURE;
        }else{
            //se la selct è andata a buon fine 
            for(i=0; i<FD_SETSIZE;++i){
                if(FD_ISSET(i,&readSet)){
                    if(i == serverSocket){
                        //nuova socket
                        int newClient;
                        newClient=accept(serverSocket, NULL, 0);
                        if(newClient==-1){
                            perror("accept()");
                            close(serverSocket);
                            execl("/bin/rm","rm -r",SOCKNAME,NULL);
                            exit(EXIT_FAILURE);
                        }
                        FD_SET(newClient, &activeSet);
                    }else{
                        //already connected client
                        string buffer = malloc(sizeof(char)*BUFFER_SIZE);
                        memset(buffer,'\0',BUFFER_SIZE);
                        printf("Server listen to Client %d:\t", i);
                        read(i, buffer,BUFFER_SIZE);
                        printf("%s----->", buffer);
                        buffer=strtoUp(buffer);
                        printf("Send back: %s\n", buffer);
                        fflush(stdout);
                        write(i,buffer,strlen(buffer)+1);
                        close(i);
                        free(buffer);
                        FD_CLR(i,&activeSet);                        
                    }
                }
            }
        }

    }  
    return 0;
}

string strtoUp (string s){
    //sistemata la funzione che traduce in maiuscole;
    for(int i=0; i<strlen(s); i++){
        //printf("DEBUG: %c", s[i]);
         s[i]=toupper(s[i]);
    }
    return s;
}