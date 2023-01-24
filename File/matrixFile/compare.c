#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char*string;

#ifndef DIM_LIMIT
#define DIM_LIMIT 512
#endif

int getIntFromString(string s);



int main (int argc, string argv[]){
    if(argc<4){
        puts("Missing amtrix dimension");       
        return -1;
    }
    if(getIntFromString(argv[1])>DIM_LIMIT){
        puts("Matrix too big");
        return -1;
    }

    int n = getIntFromString(argv[1]);

    FILE * txt;
    FILE * bin;

    txt=fopen(argv[2],"r");
    bin=fopen(argv[3],"rb");

    float ** M1 =(float**)malloc(sizeof(float*)*n);
    for (int x=0; x<n; x++){
        M1[x]=(float*)malloc(sizeof(float)*n);
    }

    float ** M2 =(float**)malloc(sizeof(float*)*n);
    for (int x=0; x<n; x++){
        M2[x]=(float*)malloc(sizeof(float)*n);
    }


    for(int x=0; x<n; x++){
        for(int y=0;y<n; y++){
            float temp =0.0;
            fread(&temp,sizeof(float), 1, bin);
            M2[x][y]=temp;
            //printf("%.1f", M2[x][y]);
        }
    }
    //puts("");
    for(int x=0; x<n; x++){
        for(int y=0;y<n; y++){
            float temp =0.0;
            fscanf(txt, "%f", &temp);
            M1[x][y]=temp;
            //printf("%.1f", M1[x][y]);
        }
    }

    for(int x=0; x<n; x++){
        for(int y=0;y<n; y++){
            if(M1[x][y]!=M2[x][y]){
                printf("L'elmento: %f riga: %d  colonna: %d  è diverso", M1[x][y],x,y);
                return -1;
            }
        }
    }
    puts("le due Matrici sono uguali");

    return 0;
}



int getIntFromString(string s){
    char * temp=NULL;
    int val = strtol(s, &temp, 0);
    return val;
}
