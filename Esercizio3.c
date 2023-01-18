#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int flag;

long isNumber(const char* s);
char* parseString(char*str,int size);

int main (int argc, char*argv[]){
    if(argc<2){
        puts("Invalid input");
        return -1;
        }
    char*output =(char*) malloc(sizeof(char)*(strlen(argv[0])+1));
    strcat(output,argv[0]);
    strcat(output," ");

    for(int i=0; i<argc; i++){
        if(strstr(argv[i], "-h")){
            printf("%s\t-n <numero>\t-s <stringa>\t-m <numero>\t-h\n", argv[0]);
            return 0;
        }
    }

    for (int j =1; j<argc; j++){
        output=(char*)realloc(output,(1+strlen(output)+strlen(parseString(argv[j],strlen(argv[j]))))*sizeof(char));
        strcat(output,parseString(argv[j],strlen(argv[j])));
    }
    
    puts(output);
    return 0;
}

char* parseString(char*str,int size){

    char*rtrn=(char*)malloc(sizeof(char));

    int i=0;
    if(str[i]=='-'){
        i++;
        while(str[i]=='-' && i<size){
            i++;
        }
        //controlliamo se è n,s,m
        switch(str[i]){
            case 'n':
            flag=1;
            if(1+i<strlen(str) && str[i+1]=='\0'){
                return "-n: ";
            }else{
                if(isNumber(&str[i+1])!=-1){
                    rtrn=(char*)realloc(rtrn,(5+strlen(&str[i+1]))*sizeof(char));
                    strcat(rtrn,"-n: ");
                    strcat(rtrn,&str[i+1]);
                    strcat(rtrn," ");
                    return rtrn;
                }else{
                    return " Not a number ";
                }
            }
            break;
            case 'm':
            flag=3;
            if(1+i<strlen(str) && str[i+1]=='\0'){
                return "-m: "; 
            }else{
                if(isNumber(&str[i+1])!=-1){
                    rtrn=(char*)realloc(rtrn,(5+strlen(&str[i+1]))*sizeof(char));
                    strcat(rtrn,"-m: ");
                    strcat(rtrn,&str[i+1]);
                    strcat(rtrn," ");
                    return rtrn;
                }else{
                    return " Not a number ";
                }
            }
            break;
            case 's':
            flag=2;
            if(1+i<strlen(str) && str[i+1]=='\0'){
                return "-m: "; 
            }else{
                    rtrn=(char*)realloc(rtrn,(5+strlen(&str[i+1]))*sizeof(char));
                    strcat(rtrn,"-s: ");
                    strcat(rtrn,&str[i+1]);
                    strcat(rtrn," ");
                    return rtrn;
            }
            break;
            default:
            return  " Unknown option ";
            break;

        }
    }else{
        if(flag%2==1 && isNumber(str)>-1){
            char*temp= malloc(sizeof(char)*(1+strlen(str)));
            strcat(temp,str);
            strcat(temp," ");
            return temp;
        }else{
            if(isNumber(str)==-1 && flag%2==1){
                return " Number error ";
            }
            if(flag%2==0){
                char*temp= malloc(sizeof(char)*(1+strlen(str)));
                strcat(temp,str);
                strcat(temp," ");
                return temp;
            }
        }
    }

}


long isNumber(const char* s) {
   char* e = NULL;
   long val = strtol(s, &e, 0);
   if (e != NULL && *e == (char)0) return val; 
   return -1;
}