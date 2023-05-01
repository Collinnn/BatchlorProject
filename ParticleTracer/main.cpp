
#include <stdio.h>
#include <iostream>
#include <stdlib.h> 
#include "particle.h"
#include <cmath>
#include <tgmath.h>
particle2D particleArr[1000*1000];
particle2D tempArr0[1000*1000];
double G = 6.67430e-11; //Gravitational constant

//Initializes the global particle array with random values
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
                double distance = sqrt(abs(temp.x-input.x+temp.y-input.y));
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
//Takes an int to choose which update method to use
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
            printf("x: %f, y: %f, x_delta: %f, y_delta: %f, weight: %f", particleArr[i+j*length].x, particleArr[i+j*length].y, particleArr[i+j*length].x_delta, particleArr[i+j*length].y_delta, particleArr[i+j*length].weight);
        }
    }
    return 0;
}

//Takes the global particle array and prints it to a ppm file
int toPPMfile(){
    int const length = 1000;
    FILE *fp;
    printf("Writing to file\n");
    fp = fopen("ParticleTracer/Output/output.ppm", "w+"); //w+ is for writing and reading
    fprintf(fp, "P3\n%d %d\n%d\n", length, length, 255);
    double Arr[length][length] = {0};
    

    //Adds together how many particles are in each pixel
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            int x = particleArr[i+j*length].x;
            int y = particleArr[i+j*length].y;
            Arr[x][y] +=1;
        }
    }

    //Prints the values of pixels to the file
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            fprintf(fp, "%d %d %d ", (int)Arr[i][j], (int)Arr[i][j], (int)Arr[i][j]);
        }
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    printf("Starting programs\n");
    initArrays();
    updateArr(0);
    toPPMfile();
    return 0;
}

