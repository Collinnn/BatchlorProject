
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <ctime> 
#include "./include/particleTypes.hpp"
#include <cmath>
#include <tgmath.h>
#include <random>
#include <cassert>

const int length = 10000;
particle2D particleArr[length];
particle2D tempArr[length];
double G = 6.67430e-11; //Gravitational constant


int initArrays(){
    int width = 100;
    int height = 100; 

    std::mt19937 gen32(0); //Standard mersenne_twister_engine seeded with 0
    std::uniform_real_distribution<double> dist{0.0, 1.0}; //Random double between 0 and 1
    std::uniform_real_distribution<double> dist2{-2.0, 2.0};
    
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            assert(i+j*width<length);
            particle2D input = particleArr[i+j*width];
            input.x = gen32()%width;
            input.y = gen32()%height;
            input.x_delta = dist2(gen32); //Random speed values
            input.y_delta = dist2(gen32); //Random speed values
            input.weight  = dist(gen32);   //Random weight 0-1
            particleArr[i+j*width] = input;
        }
    }
    return 0;   
}


int gravtiationalForceUpdate(int x){
    particle2D input= particleArr[x];
    
    for (int i = 0; i <= length; i++)
    {
        particle2D temp = particleArr[i];
        double xdiff = input.x-temp.x;
        double ydiff = input.y-temp.y;
        double distance = (double) sqrt(abs(xdiff*xdiff+ydiff*ydiff)); //Distance between two particles, done right
        double force = G*temp.weight*input.weight/(distance*distance); //Force between two particles
        double acceleration = force/input.weight; //Acceleration of the particle
        double xc = xdiff/distance; //Cosine of the angle between the two particles
        double yc = ydiff/distance; //Sine of the angle between the two particles
        input.x_delta -= acceleration*xc;
        input.y_delta -= acceleration*yc;
    }
    //printf("Done with particle %d, %d\n", x, y);

    tempArr[x] = input;
    
    return 0;
}

int updateToNewCordinates(){
    for (int i = 0; i <= length; i++)
    {
        particle2D temp = particleArr[i];
        temp.x += temp.x_delta;
        temp.y += temp.y_delta;
        tempArr[i] = temp;
    }
    return 0;
}

int bruteForceUpdate(){
    printf("Starting to update\n");
    for (int i = 0; i <= length; i++)
    {
        gravtiationalForceUpdate(i);
        
    }
    updateToNewCordinates();

     
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
    for (int i = 0; i <= length; i++)
    {
        printf("x: %f, y: %f, x_delta: %f, y_delta: %f, weight: %f \n", particleArr[i].x, particleArr[i].y, particleArr[i].x_delta, particleArr[i].y_delta, particleArr[i].weight);
        //printf("weight: %f", particleArr[i].weight);
    }
    return 0;
}

//Takes the global particle array and prints it to a ppm file
int toPPMfile(){
    int height = 100;
    int width = 100;
    std::remove("./Output/output.ppm");
    std::ofstream fout("./Output/output.ppm");
    if(fout.fail()){
        std::cout << "Error opening file" << std::endl;
        return -1;
    }

    fout << "P3\n";
    fout << height << " " << width << "\n";
    fout << "255\n";

    double Arr[length] = {0};
    int x = 0;
    int y = 0;
    int z = 0;


    //Adds together how many particles are in each pixel
    for (int i = 0; i < length; i++)
    {
        assert(particleArr[i].x==tempArr[i].x);
        x = particleArr[i].x;
        y = particleArr[i].y;
        z= x+y*width;
        if(z<length && z>0){
            Arr[z] +=1;
        }
       
    }
    printf("Done adding\n");
    //Prints the values of pixels to the file
    for (int i = 0; i < length; i++)
    {
        printf("%d %d %d ", (int)Arr[i]*10, (int)Arr[i]*10, (int)Arr[i]*10);
        fout << (int)Arr[i]*10 << " " << (int)Arr[i]*10 << " " << (int)Arr[i]*10 << "  ";
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

