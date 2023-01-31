#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>


typedef char* string;

int toNumber(string n);
void errorMessage(string s);

int main (int argc, string argv[]){
    if(argc!=2){
        errorMessage("Missing timeWait argument");
        return 1;
    }
    int time = toNumber(argv[1]);
    if(time==-1){
        errorMessage("Invalid time argument");
        return 1;
    }
    int pid,status;

    pid=fork();
    if(pid==0){
        if((pid=fork())==0){
            if(execl("/bin/sleep", "/bin/sleep,", argv[1], NULL)==-1){
                perror("execl");
                exit(EXIT_FAILURE);
            } 
        }
        if(waitpid(pid, NULL, 0)==-1){
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "\nPID figlio= %d\nPID padre= %d\n", getpid(), getppid());
        return 0;
    }
    return 0;
}

void errorMessage(string s){
    fprintf(stderr,"%s", s);
}

int toNumber(string n){
    string e=NULL;
    int num=strtol(n, &e, 0);
    if(*e==(char)0 && e!=NULL){
        return num;
    }
    return-1;
}