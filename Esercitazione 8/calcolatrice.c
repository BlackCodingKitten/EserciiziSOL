#define _POSIX_C_SOURCE 200112L
#define _OPEN_SYS_ITOA_EXT
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define INPUT_SIZE 128
#define OUTPUT_SIZE 23

typedef char* string ;

void calcolatriceScientifica(string input, size_t dim);
//size_t clearString(string input);

int main (void){
    string input ;
    while (true){
        input = malloc(sizeof(char)*INPUT_SIZE);
        fprintf(stdout, "Operazione: ");
        fgets(input,INPUT_SIZE,stdin);
        if(strstr(input,"quit")){
            break;
        }
        //printf("%s\t%ld\n", input,strlen(input));
        calcolatriceScientifica(input,strlen(input));
        // free(input);
    }
    free(input);
    return EXIT_SUCCESS;
}

void calcolatriceScientifica(string input, size_t dim){
    string arg = malloc(sizeof(char)*dim);
    strncpy(arg,input,dim);
    //puts(arg);
    string res=malloc(sizeof(char)*OUTPUT_SIZE);
    int pfd1[2],pfd2[2];
    //pipe 1 figlio     pipe 2 padre
    pipe(pfd1);
    pipe(pfd2);

    pid_t id=fork();

    switch(id){
        case -1 :
            fprintf(stderr,"process id\n");
            exit(EXIT_FAILURE);
            break;
        case 0 :
            //child
            close(pfd1[1]);
            close(pfd2[0]);
            //0 input 1 output
            dup2(pfd1[0],STDIN_FILENO);
            dup2(pfd2[1],STDOUT_FILENO);
            close(pfd1[0]);
            close(pfd2[1]);

            execlp("bc","bc",NULL);
            exit(EXIT_FAILURE);
            break;
        default:
            close(pfd1[0]);
            close(pfd2[1]);

            if(write(pfd1[1], arg, sizeof(arg)-1) != sizeof(arg)-1){
                fprintf(stderr, "write fail\n");
                close(pfd1[1]);
                close(pfd2[0]);
                exit(EXIT_FAILURE);
            }

            if(read(pfd2[0], res, sizeof(res))<0){
                fprintf(stderr, "read from child process fail\n");
                close(pfd1[1]);
                close(pfd2[0]);
                exit(EXIT_FAILURE);
            }else{
                fflush(stdout);
                printf("Risualtato: %s", res);
                fflush(stdout);
                free(res);
            }
            close(pfd1[1]);
            close(pfd2[0]);

    }
    free(arg);
    printf("\n");
    return;
}