#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define LIMIT 1000
#define WORD_COUNT 1
#define LINE_COUNT 2
#define ERROR -1

typedef char* string;
typedef FILE* file;

int wc (string path);
int lc (string path);
int evalueOption(string*a, int dim);

/************MAIN*************/
int main(int argc, string argv[]){
    if(argc<1){
        puts("Missing file");
        return ERROR; 
    }
    if(!strstr(argv[1],"-")){
        for(int i=1; i<argc; i++){
            printf("%s\t-l: %d   -w: %d\n", argv[i],lc(argv[i]),wc(argv[i]));
        }
        return 0;
    }
    switch(evalueOption(argv, argc)){
        case WORD_COUNT:
            for(int i=2; i<argc; i++){
                printf("%s\t-w: %d\n", argv[i],wc(argv[i]));
            }
            break;
        case LINE_COUNT:
            for(int i=2; i<argc; i++){
                printf("%s\t-l: %d\n", argv[i],lc(argv[i]));
            }
            break;
        case ERROR:
            printf("Unknown option \"%s\"\n", argv[1]);
            return ERROR;
            break;
        default:
            puts("Unknown Error");
            return ERROR;
            break;
    }
       
    return 0;
}
/**********FINE MAIN**********/

int wc(string path){
    file f=fopen(path, "r");
    int c=0;
    while(!feof(f)){
        string s = malloc(sizeof(char)*LIMIT+1);
        fgets(s,LIMIT,f);
        for(int i=0; i<(strlen(s)-1); i++){
            if(isspace(s[i])){
                c++;
            }
        }
    }
    fclose(f);
    return c;
}

int lc(string path){
    file f=fopen(path, "r");
    int c=0;
    while(!feof(f)){
        string s = malloc(sizeof(char)*LIMIT+1);
        fgets(s,LIMIT,f);
        for(int i=0; i<strlen(s); i++){
            if(iscntrl(s[i])){
                c++;
            }
        }
    }
    fclose(f);
    return c;
}

int evalueOption(string * a, int dim){
    opterr=0;
    int c=0;

    while ((c = getopt(dim, a, "l:w:"))!=1){
        switch(c){
            case 'l':
                return LINE_COUNT;
                break;
            case 'w' :
                return WORD_COUNT;
                break;
            case '?':
            default:
                return ERROR;
                break;                
        }
    }

}