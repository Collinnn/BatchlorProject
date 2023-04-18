
#include <stdio.h>
#include <iostream>
#include <stdlib.h> 
#include "particle.h"
#include <cmath>
#include <tgmath.h>
particle2D particleArr[1000*1000];
particle2D tempArr0[1000*1000];
double G = 6.67430e-11; //Gravitational constant

int initArrays(){
    int length = 1000;
    
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            particleArr[i+j*length].x = i;
            particleArr[i+j*length].y = j;
            particleArr[i+j*length].x_delta = rand()%6-3; //Random speed values
            particleArr[i+j*length].y_delta = rand()%6-3; //Random speed values
            particleArr[i+j*length].weight  = rand()%3;   //Random weight
        }
    }

    return 0;   
}

int gravtiationalForceUpdate(int x,int y){
    particle2D input= particleArr[x+y*1000];
    
    for (int i = 0; i <= 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
                particle2D temp = particleArr[x+y*1000];
                double distance = sqrt(temp.x*temp.x+temp.y*temp.y);
                double force = G*temp.weight*input.weight/(distance*distance);
                double angle = atan2(input.y-temp.y,input.x-input.y);
                input.x_delta += force*cos(angle);
                input.y_delta += force*sin(angle);
        }
    }
    particleArr[x+y*1000] = input;
    return 0;
}

int bruteForceUpdate(){
    int length = 1000;
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            gravtiationalForceUpdate(i,j);
        }
    }
    return 0;
}

int PICUpdate(){
    return 0;
}

int updateArr(int type){
   if(type == 0){
     bruteForceUpdate();
   }
   else if (type == 1){
    PICUpdate();
   }

    return 0;
}
int printArr(){
    int length = 1000;
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            printf("x: %f, y: %f, x_delta: %f, y_delta: %f, weight: %f");
        }
    }
    return 0;
}


int main(int argc, char const *argv[])
{
    initArrays();
    updateArr(0);
    return 0;
}

