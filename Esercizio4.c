#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

long isNumber(const char* s);
char* evalue (int cIndex, char*v[]);

int main(int argc,  char*argv[]){
    
    //se c'è un messaggio di -h:help ignoro tutta la riga e restituisco solo il menù help
    for (int i=1; i <argc; i++){
        if(strstr(argv[i], "-h")){
            printf("%s: -n <numero> -s <stringa> -m <numero> -h\n", argv[0]);
            return 0;
        }
    }
    puts(evalue(argc,argv));
    return 0;

}

char*evalue (int cIndex, char*v[]){
    char*rtrn =malloc(sizeof(char)*strlen(v[0]));
    strcat(rtrn, v[0]);
    int c;
    char*arg=NULL;
    //disattiva il messaggio di errore automatico di getopt
    opterr=0;
    //pars della linea di comando
    while ((c = getopt(cIndex, v, "n:m:s:"))!=-1){
        switch(c){
            case 'n':
                arg=optarg;
                rtrn=(char*)realloc(rtrn,(strlen(rtrn)+4)*sizeof(char));
                strcat(rtrn," -n: ");
                if(arg==NULL){
                    rtrn=(char*)realloc(rtrn,(strlen(rtrn)+strlen("Argomento mancante "))*sizeof(char));
                    strcat(rtrn,"Argomento mancante ");
                    break;
                }
                if(isNumber(arg)==-1){
                    rtrn=(char*)realloc(rtrn,(strlen(rtrn)+strlen("NaN "))*sizeof(char));
                    strcat(rtrn,"NaN ");
                }else{
                    rtrn=(char*)realloc(rtrn,(strlen(rtrn)+strlen(arg))*sizeof(char));
                    strcat(rtrn,arg);
                }
                break;
            case 'm':
                arg=optarg;
                rtrn=(char*)realloc(rtrn,(strlen(rtrn)+4)*sizeof(char));
                strcat(rtrn," -m: ");
                if(arg==NULL){
                    rtrn=(char*)realloc(rtrn,(strlen(rtrn)+strlen("Argomento mancante "))*sizeof(char));
                    strcat(rtrn,"Argomento mancante ");
                    break;
                }
                if(isNumber(arg)==-1){
                    rtrn=(char*)realloc(rtrn,(strlen(rtrn)+strlen("NaN "))*sizeof(char));
                    strcat(rtrn,"NaN ");
                }else{
                    rtrn=(char*)realloc(rtrn,(strlen(rtrn)+strlen(arg))*sizeof(char));
                    strcat(rtrn,arg);
                }
                break;
            case 's':
                arg=optarg;
                rtrn=(char*)realloc(rtrn,(strlen(rtrn)+4)*sizeof(char));
                strcat(rtrn," -s: ");
                if(arg==NULL){
                    rtrn=(char*)realloc(rtrn,(strlen(rtrn)+strlen("Argomento mancante "))*sizeof(char));
                    strcat(rtrn,"Argomento mancante ");
                    break;
                }
                rtrn=(char*)realloc(rtrn,(strlen(rtrn)+strlen(arg))*sizeof(char));
                strcat(rtrn,arg);
                break;
            case '?':
                break;
            default:
                abort();
            
        }
    }//end while
    return rtrn;
}



long isNumber(const char* s) {
   char* e = NULL;
   long val = strtol(s, &e, 0);
   if (e != NULL && *e == (char)0) return val; 
   return -1;
}