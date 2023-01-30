#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>


#define MAX_LENGTH 100
#define MAX_ARGS 256


typedef char* string;



void printShell (void);
int read_cmd_line(string input, string*argv);
void freeMem(string*a);


int main (void){
    int pid, status;
    string input=(string)malloc (sizeof(char)*MAX_LENGTH);
    string *argv=(string*)malloc(sizeof(string)*MAX_ARGS);
    while(1){
        printShell();
        memset(input,0,MAX_LENGTH);

        int tmp = read_cmd_line(input, argv);
        switch(tmp){
            case 0:
                pid=fork();
                switch(pid){
                    case -1:
                        //errore
                        perror("fork");
                        exit(EXIT_FAILURE);
                    case 0:
                        //processo figlio
                        execvp(argv[0], &(argv[0]));
                        perror("execvp");
                        exit(errno);
                    default:
                        if((pid =waitpid(-1,&status,0))==-1){
                            perror("waitpid");
                            exit(EXIT_FAILURE);
                        }else if(pid !=0){
                            if(WIFEXITED(status)){
                                fprintf(stdout,"Process %d ended with status %d\n", pid, WEXITSTATUS(status));
                                fflush(stdout);
                            }
                        }
                        freeMem(argv);
                }
                break;
            case -1:
                exit(EXIT_FAILURE);
            case 1:
                fprintf(stdout, "no command entered\n");
                break;
        }
    }
    

    printf("\n");
    return 0;
}

void freeMem(string*a){
    for(int i=0; i<sizeof(a); i++){
        free(a[i]);
    }
}

int read_cmd_line(string input, string*argv){

    if(input == NULL){
        fprintf(stderr, "No input found");
        return -1;
    }
    if(fgets(input, MAX_LENGTH, stdin)==NULL){
        perror("fgets");
        return -1;
    }
    if(strstr(input,"exit")){
        fprintf(stdout,"Uscita in corso...\n");
        free(input);
        exit(EXIT_SUCCESS);
    }
    string temp,token;
    token=strtok_r(input, " ", &temp);
    int i=0 , len =0;
    len = strlen(token)+1;
    argv[i]=(string)malloc(sizeof(char)*len);
    strncpy(argv[i], token, len);

    while((token =strtok_r(NULL, " ", &temp))!=NULL){
        if(token[0]!='\0' && token[0]!='\n'){
            i++;
            len = strlen(token)+1;
            argv[i]=(string)malloc(sizeof(char)*len);
            //elimino l'accapo
            token[strcspn(token,"\n")]='\0';
            strncpy(argv[i], token, len);

        }   
    }

    if(i==0){

        return 1;
    }
    argv[i+1]=NULL;
    for(int y=0; y <=i; y++ ){
        fprintf(stdout, "argv[%d] = %s\n", y , argv[y]);
    }
    return 0;

}   


void printShell (void){
    printf("Laboratorio/Sol/Esercitazione6/dummyshell.c>> ");
}
