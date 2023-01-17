#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void stdupper (const char * i, size_t len, char* out);

int main (int argc, char*argv[]){
    char* out = malloc(sizeof(char)*strlen(argv[1]));
    stdupper(argv[1], (size_t)strlen(argv[1]), out);
    puts(out );

    return 0;
}

void stdupper (const char * i, size_t len, char* out){
    for (size_t k=0; k<len; k++){
      out[k] = toupper(i[k]);
    }
}