#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
 
#define BUFF_SIZE 50

typedef char*string;
int CtrlC=0;
int CtrlZ=0;
static void gestore (int sig);
static void gestoreAlarm (int sig);

int main (void){
    //sa_handler indica come gestire il seganle : SIG_ING serve per ignorare 
    //SIG_DFL usa la gestione di funzione di default 
    struct sigaction s;
    memset(&s,0,sizeof(s));
    s.sa_handler = gestore;
    //installo nuovo gestore su ctrl+c
    if(sigaction(SIGINT,&s,NULL) == -1){
        perror("sigaction(SIGINT)");
        exit(EXIT_FAILURE);
    }
    //lo associo anche a ctrl+z
    if(sigaction(SIGTSTP,&s,NULL)==-1){
        perror("sigaction(SIGSTP)");
        exit(EXIT_FAILURE);
    }
    //gestione dopo lo scadere dell'alarm
    struct sigaction timeout;
    memset(&timeout,0,sizeof(timeout));
    timeout.sa_handler = gestoreAlarm;
    if(sigaction(SIGALRM,&timeout,NULL)==-1){
        perror("sigaction(SIGALRM)");
        exit(EXIT_FAILURE);
    }
    while(1){
        continue;
    }
    return EXIT_SUCCESS;
}
//la printf non è garantita signal safe, nemmeno la scanf o la funzione exit
//per lettura e scrittura usare funzioni tipo la wite e la read su stdin/stdout_fileno, 
//per uscire usare la '_exit()'

static void gestoreAlarm (int sig){
    write(STDERR_FILENO,"Mancata risposta, errore\n",strlen("Mancata risposta, errore\n")+1);
    _exit(1);
}

void gestore(int sig){
    string buffer = malloc(sizeof(char)*BUFF_SIZE);
    switch(sig){
        case SIGINT:
            CtrlC++;
            break;
        case SIGTSTP:
            CtrlZ++;
            sprintf(buffer, "%d", CtrlC);
            buffer[strlen(buffer)]='\n';
            write(STDOUT_FILENO, "\n", strlen("\n")+1);
            write(STDOUT_FILENO, buffer, strlen(buffer)+1);
            memset(buffer,'\0', BUFF_SIZE);
            if(CtrlZ==3){
                write(STDOUT_FILENO, "Continuare?\n", strlen("Continuare?\n")+1);
                alarm(10);
                while(1){
                    if(read(STDIN_FILENO,buffer,BUFF_SIZE)>0){
                        alarm(0);
                        if(strstr(buffer,"si")){
                            CtrlZ=0;
                            return;
                        }else{
                            break;
                        }
                    }
                }
                free(buffer);
                _exit(0);
            }
            break;
        default:
            break;
    }    
}