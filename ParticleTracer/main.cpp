
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include "./include/particle.h"
#include <cmath>
#include <tgmath.h>
particle2D particleArr[100*100];
particle2D tempArr[100*100];
double G = 6.67430e-11; //Gravitational constant




//Initializes the global particle array with random values
int initArrays(){
    int length = 100;
    
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

int gravtiationalForceUpdate(int x,int y, int lengthx, int lengthy){
    particle2D input= particleArr[x+y*100];
    
    for (int i = 0; i <= lengthx; i++)
    {
        for (int j = 0; j < lengthy; j++)
        {
                particle2D temp = particleArr[x+y*100];
                double distance = (double) sqrt(abs(temp.x-input.x+temp.y-input.y));
                double force = G*temp.weight*input.weight/(distance*distance);
                double angle = atan2(input.y-temp.y,input.x-input.y);
                input.x_delta += force*cos(angle);
                input.y_delta += force*sin(angle);
        }
        
    }
    //printf("Done with particle %d, %d\n", x, y);

    tempArr[x+y*100] = input;
    return 0;
}

int bruteForceUpdate(){
    int length = 100;
    printf("Starting to update\n");
    for (int i = 0; i <= length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            gravtiationalForceUpdate(i,j,length,length);
        }
    }
    std::copy(std::begin(tempArr), std::end(tempArr), std::begin(particleArr));
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
    int length = 100;
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

    int const length = 100;
    std::ofstream fout("output.ppm");
    if(fout.fail()){
        std::cout << "Error opening file" << std::endl;
        return -1;
    }

    fout << "P3\n";
    fout << length << " " << length << "\n";
    fout << "255\n";

    double Arr[1000][1000] = {0};
    

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
    printf("Done adding\n");
    //Prints the values of pixels to the file
    for (int i = 0; i <= 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            printf("%d %d %d ", (int)Arr[i][j], (int)Arr[i][j], (int)Arr[i][j]);
            fout << (int)Arr[i][j] << " " << (int)Arr[i][j] << " " << (int)Arr[i][j] << "  ";
        }
    }
    fout.close();
    return 0;
}

int main(){
    printf("Starting programs\n");
    printf("Initializing arrays\n");

    initArrays();
    printArr();
    printf("Updating arrays\n");
    updateArr(0);
    printf("Printing arrays to ppm\n");
    printArr();
    toPPMfile();
    printf("Done\n");
    return 0;
}

