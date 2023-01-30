#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>



typedef char* string;
typedef struct stat INFO;

int searchFile(string dirName, string fileNames);

int main (int argc, string argv[]){
    if(argc !=3  ){
        fprintf(stderr,"Usage: %s 'dir' 'filename'.", argv[0]);
        return EXIT_FAILURE;
    }

    INFO infoDir;
    if(stat(argv[1],&infoDir)==-1){
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    if(!S_ISDIR(infoDir.st_mode)){
        fprintf(stderr,"%s is not a directory", argv[1]);
        return EXIT_FAILURE;
    }

    if(searchFile(argv[1] , argv[2])!=0){
        puts("file non presente nella cartella");
    }
    return 0;
}

int searchFile(string dirName, string fileName){
    //apro la cartella di nome 'dirName'
    string save=dirName;
    DIR * dir;
    if((dir=opendir(dirName))==NULL){
        perror("Impossibile aprire la cartella ");
        exit(EXIT_FAILURE);
    }
    //esploro la cartella
    struct dirent * dirPtr;
    INFO statinfo;
    while((errno=0, dirPtr=readdir(dir))!=NULL){
        //escludo le cartelle . e ..
        if(dirPtr->d_name[0]=='.'){
            continue;
        }
        string path = (string) malloc (sizeof(char)*PATH_MAX);
        strcpy(path,dirName);
        strcat(path,"/");
        strcat(path, dirPtr->d_name);

        INFO info;
        if(stat(path, &info)==-1){
            perror(path);
            exit(EXIT_FAILURE);
        }
        if(strcmp(dirPtr->d_name,fileName)==0){
            fprintf(stdout, "%s Ultima Modifica: %s", path,ctime(&statinfo.st_mtime));
            return 0;
        }
        if(S_ISDIR(info.st_mode)){
            if(searchFile(path,fileName)==0){
                return 0;
            }else{
                free(path);
                continue;
            }
        }
        
    }
    closedir(dir);
    return -1;
}