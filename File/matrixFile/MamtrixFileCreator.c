#include <stdio.h>
#include <stdlib.h>

#ifndef DIM_LIMIT
#define DIM_LIMIT 512
#endif

typedef char*string;

int getIntFromString(string s);
void fillMatrix(int dim, float**Ptr);
void ftoa(float n, string res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);
float Pow(float base, float exp);

int main (int argc, string argv[]){
    if(argc<2){
        puts("Missing amtrix dimension");       
        return -1;
    }
    if(getIntFromString(argv[1])>DIM_LIMIT){
        puts("Matrix too big");
        return -1;
    }
    float ** matrix =(float**)malloc(sizeof(float*)*getIntFromString(argv[1]));
    for (int x=0; x<getIntFromString(argv[1]); x++){
        matrix[x]=(float*)malloc(sizeof(float)*getIntFromString(argv[1]));
    }

    fillMatrix(getIntFromString(argv[1]),matrix);

    FILE* outputBin;
    FILE *outputTxt;

    outputTxt=fopen("./matrix.txt","w");
    outputBin=fopen("./matrix.dat","wb");

    for(int x=0; x <getIntFromString(argv[1]); x++){
        for(int y=0;  y<getIntFromString(argv[1]); y++){
            string temp= malloc(4* sizeof(char));
            ftoa(matrix[x][y],temp,1);
            fprintf(outputTxt, "0%s  ", temp);
            float  t = matrix[x][y];
            fwrite(&t,sizeof(t),1,outputBin);
        }
        fprintf(outputTxt, "%s", "\n");
    }


    return 0;
}

int getIntFromString(string s){
    char * temp=NULL;
    int val = strtol(s, &temp, 0);
    return val;
}

void fillMatrix(int dim, float**Ptr){
    for(int y=0; y<dim; y++){
        for(int x=0; x<dim; x++){
            Ptr[y][x]= (y+x)/2.0;
        }
    }
}

void ftoa(float n, string res, int afterpoint){
    int ipart = (int)n;
    float fpart = n - (float)ipart;
    int i = intToStr(ipart, res, 0);
    if (afterpoint != 0) {
        res[i] = '.';
        fpart = fpart * Pow(10, afterpoint);
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

float Pow(float base, float exp){
    float acc=base;
    for(int i=1; i<exp; i++ ){
        acc= acc*base;
    }
    return acc;
}

void reverse(string str, int len){
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}


int intToStr(int x, char str[], int d){
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}