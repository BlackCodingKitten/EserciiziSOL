#include <stdio.h>
#include <stdlib.h>
#define DEFAULT_SIZE 256

typedef char* string;
typedef FILE*  file;



int main (int argc, string argv[]){
    file in = fopen(argv[1], "r");
    file out = fopen(argv[2], "w");

    int dim =DEFAULT_SIZE;
    if(argc ==4){
        string temp=NULL;
        dim = strtol(argv[3], &temp, 0);
    }

    string s = malloc(sizeof(char)*dim);

    while(!feof(in)){
        fread(s, dim, 1, in);
        fwrite(s, dim, 1, out);
    }
    
    fclose(in),
    fclose(out);
    free(s);

    return 0;
}