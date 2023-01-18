#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

const int REALLOC_INC=16;

void RIALLOCA (char** buf, size_t newsize);
char* mystrcat(char* buffer, size_t sz, char* first, ...);

int main(int argc, char *argv[]) {
  if (argc < 7) { printf("troppi pochi argomenti\n"); return -1; }
    if (argc > 7) { printf("troppi argomenti\n"); return -1; }
  
  char *buffer=NULL;
  RIALLOCA(&buffer, REALLOC_INC);  // macro che effettua l'allocazione del 'buffer'
  buffer[0]='\0'; // mi assicuro che il buffer contenga una stringa
  buffer = mystrcat(buffer, REALLOC_INC, argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL);
  printf("%s\n", buffer);     
  free(buffer);
  return 0;
}


char* mystrcat(char* buffer, size_t sz, char* first, ...){
    va_list str_list;
    va_start(str_list, first);
    char* str = malloc(sizeof(char)*strlen(first));
    str=first;
    do{
        RIALLOCA (&buffer, (size_t)(strlen(buffer)+REALLOC_INC));
        strcat(buffer,str);
        str = va_arg(str_list, char*);        
    }while ((str!=NULL));
    va_end(str_list); 
    return buffer;  
}

void RIALLOCA(char** buf, size_t newsize){
    *buf = (char*)realloc(*buf, newsize);
}