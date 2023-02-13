
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
#define SOCKNAME "./socket3"
#define MAX_CLIENT 100
#define BUFFER_SIZE 150


typedef char* string;


int main (int argc, string argv[]){
    struct sockaddr_un addr;

    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path, SOCKNAME, UNIX_PATH_MAX);

    int fd;
    fd=socket(AF_UNIX,SOCK_STREAM, 0);
    if(fd==-1){
        perror("socket()");
        exit(EXIT_FAILURE); 
    }

    while(connect(fd,(struct sockaddr*)&addr, sizeof(addr))==-1){
        puts("Attesa di connessione");
        if(errno==ENOENT){
            sleep(1);
        }else{
            return EXIT_FAILURE;
        }
    }

    size_t dim =strlen(argv[1])+1;
    write(fd, argv[1], dim);
    memset(argv[1], '\0', strlen(argv[1]));
    read(fd, argv[1], dim);
    fprintf(stdout, "%s\n", argv[1]);
    close(fd);
       
    return 0;
}