
#include <stdio.h>
#include <stdlib.h> 
#include "particle.h"
particle2D particleArr[1000*1000];

int initArrays(){
    int length = 1000;
    
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            particleArr[i+j*length].x = i;
            particleArr[i+j*length].y = j;
            particleArr[i+j*length].x_delta = rand()%6-3;
            particleArr[i+j*length].y_delta = rand()%6-3;
            particleArr[i+j*length].weight  = rand()%3;
        }
    }
    
    
    
    return 0;   
}


int main(int argc, char const *argv[])
{
    initArrays();
    return 0;
}

