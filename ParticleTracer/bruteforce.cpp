#include <stdio.h>
#include "./include/particleTypes.hpp"
#include "./include/particles.hpp"
double G = 6.67430e-11; //Gravitational constant

/*
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
*/

int bruteForceUpdate(){
    /*
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
    */
    return 0;
}
