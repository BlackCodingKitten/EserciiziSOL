
#include <stdlib.h>

int sum_r (int x,int*ptr){
    *ptr+=x;
    return *ptr;
}