#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <string.h>

typedef char* string;
typedef struct stat INFO;

int main (void){
    string path1 ="sdfyhasdhfbasdbnasdnadnakjdgattosdgsgsgs";
    string path2 ="gatto";
 
    printf("%s", strpbrk(path1,path2));
    return 0;
}