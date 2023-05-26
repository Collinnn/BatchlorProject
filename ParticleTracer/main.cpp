
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <ctime> 
#include "./include/particleTypes.hpp"
#include "./include/particles.hpp"
#include <cmath>
#include <tgmath.h>
#include <random>
#include <cassert>



double G = 6.67430e-11; //Gravitational constant


int initArrays(){
    int width = 100;
    int height = 100; 

    std::mt19937 gen32(0); //Standard mersenne_twister_engine seeded with 0
    std::uniform_real_distribution<double> dist1{0.1, 1.0}; //Random double between 0 and 1
    std::uniform_real_distribution<double> dist2{-2.0, 2.0};
    std::uniform_real_distribution<double> dist3{0.0, (double) width};

   
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            assert(i+j*width<particles::getLength());
            particle2D input; //here
            input.x = dist3(gen32);
            input.y = dist3(gen32);
            input.x_delta = dist2(gen32); //Random speed values
            input.y_delta = dist2(gen32); //Random speed values
            input.weight  = dist1(gen32);   //Random weight 0-1
            particles::setParticleArrayValue(i+j*width,input); // here
            
        }
    }
    return 0;   
}

int printArr(){
    particle2D* output = particles::getParticleArray();
    for (int i = 0; i < particles::getLength(); i++)
    {
        printf("x: %f, y: %f, x_delta: %f, y_delta: %f, weight: %f \n",
        output[i].x, output[i].y, output[i].x_delta, output[i].y_delta, output[i].weight);
    }
    return 0;
}


particle2D gravtiationalForceUpdate(int x,particle2D tempArr){
    particle2D input = particles::getParticleArrayValue(x);

    double x_delta = input.x_delta;
    double y_delta = input.y_delta;
    for (int i = 0; i < particles::getLength(); i++){
        particle2D temp = particles::getParticleArrayValue(i);

        if(input.x == temp.x && input.y == temp.y){
            continue;
        }else{
            double xdiff = input.x-temp.x;
            double ydiff = input.y-temp.y;

            double distance = sqrt(abs(xdiff*xdiff+ydiff*ydiff)); //Distance between two particles, done right
            if(distance == 0){
                continue;
            }else{
            double force = G*temp.weight*input.weight/(distance*distance); //Force between two particles
            double acceleration = force/input.weight; //Acceleration of the particle
            double xc = xdiff/distance; //Cosine of the angle between the two particles
            double yc = ydiff/distance; //Sine of the angle between the two particles

            x_delta = x_delta + acceleration*xc;
            y_delta = y_delta + acceleration*yc;
            }
        } 
    }

    tempArr.x_delta = x_delta;
    tempArr.y_delta = y_delta;
    return tempArr;
}

int updateToNewCordinates(particle2D* tempArr){
    particle2D* particleArr = particles::getParticleArray();
    for (int i = 0; i < particles::getLength(); i++)
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
    particle2D * tempArr = particles::getParticleArray();

    for (int i = 0; i < particles::getLength(); i++)
    {
        tempArr[i]=gravtiationalForceUpdate(i, tempArr[i]);
    }
    
    updateToNewCordinates(tempArr);

    for (int i = 0; i < particles::getLength(); i++)
    {
        particles::setParticleArrayValue(i,tempArr[i]);
    }
    particles::setParticleArray(tempArr);
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
    
    double Arr[10000] = {0}; //This is hardcoded to 100x100, should be changed to be dynamic
    int x = 0;
    int y = 0;
    int z = 0;

    particle2D * particleArr = particles::getParticleArray();

    //Adds together how many particles are in each pixel
    for (int i = 0; i < particles::getLength(); i++)
    {
        x = particleArr[i].x;
        y = particleArr[i].y;
        z= x+y*width;
        if(z<particles::getLength() && z>0){
            Arr[z] +=1;
        }
       
    }
    printf("Done adding\n");
    //Prints the values of pixels to the file
    for (int i = 0; i < particles::getLength(); i++)
    {
        //printf("%d %d %d ", (int)Arr[i]*10, (int)Arr[i]*10, (int)Arr[i]*10);
        fout << (int)Arr[i]*10 << " " << (int)Arr[i]*10 << " " << (int)Arr[i]*10 << "  ";
    }
    fout.close();
    return 0;
}

int main(){
    printf("Starting programs\n");
    printf("Initializing arrays\n");
    initArrays();
    //printArr();
    printf("Updating arrays\n");
    updateArr(0);
    printf("Printing arrays to ppm\n");
    //printArr();

    toPPMfile();
    printf("Done\n");

    return 0;
}

