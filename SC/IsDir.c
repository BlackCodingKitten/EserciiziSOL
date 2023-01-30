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

int exploreDir(string dir);

string printPermessi (struct stat info);

int main(int  argc,  string argv[ ]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s dir\n", argv[0]);
        return EXIT_FAILURE;
    }


    struct stat infoDir;
    if(stat(argv[1], &infoDir)==-1){
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    if(!S_ISDIR(infoDir.st_mode)){
        fprintf(stderr,"%s is not a directory", argv[1]);
        return EXIT_FAILURE;
    }

    exploreDir(argv[1]);
    return 0;

}


string printPermessi (struct stat info){
    string ret =malloc(6*sizeof(char));
    if (S_IRUSR & info.st_mode){
        ret[0]='r';
    } 
    else {
        ret[0]='-';
    }
    if (S_IWUSR & info.st_mode){ 
        ret[1]='w';
    }
    else{
        ret[1]='-';
    } 
    if (S_IXUSR & info.st_mode){
        ret[2]='x';
    } 
    else {
        ret[2]='-';
    }
    if (S_IRGRP & info.st_mode){
        ret[3]='r';
    }else{
        ret[3]='-';
    }
    if(S_IWGRP & info.st_mode){
        ret[4]='w';
    }else{
        ret[4]='-';
    }
    if(S_IXGRP & info.st_mode){
        ret[5]='x';
    }else{
        ret[5]='-';
    }
    return ret;
}

int exploreDir(string dir){
    fprintf(stdout, "\n<Nella directory %s>\n\n", dir);
    DIR * d;
    if((d=opendir(dir))==NULL){
        perror(dir);
        exit(EXIT_FAILURE);
    }


    string* othdir=(string*)malloc(sizeof(string)*256);

    
    struct dirent* f;
    struct stat info;
    int counter =0;

    
    while ((errno=0 , f=readdir(d))){
        if(f->d_name[0]=='.'){

            continue;
        }

        string path = (char*)malloc(sizeof(char)*PATH_MAX);
        strcpy(path, dir);
        strcat(path, "/");
        strcat(path, f->d_name);

        if(stat(path, &info)==-1){
            perror(path);
            return EXIT_FAILURE;
        }
        if(S_ISDIR(info.st_mode)){

            othdir[counter]=(string)malloc(sizeof(char)*PATH_MAX);
            othdir[counter]=f->d_name;
            counter++;
        }

        fprintf(stdout,"%s\t%ld\t%ld%s\n", f->d_name,info.st_size,(long)info.st_ino,printPermessi(info));
        free(path);
    }
    fprintf(stdout,"\n.............................................\n");
    for(int x=0; x<counter;x++){
        string temp=(char*)malloc(sizeof(char)*PATH_MAX);
        strcpy(temp,dir);
        strcat(temp,"/");
        strcat(temp,othdir[x]);
        exploreDir(temp);
        free(temp);
    }
}

