#include <stdio.h>
#include "./include/particleTypes.hpp"
#include "./include/particles.hpp"
//#define MY_DEBUG
double G = 6.67430e-11; //Gravitational constant

void gravtiationalForceUpdate(int x,particle_storage& storage){
    particle3D input = storage[x];

    
    int size = storage.getSize();
    for (int i = 0; i < size; i++){
        particle3D temp = storage[i];

        if(input.x == temp.x && input.y == temp.y){
            continue;
        }else{
            double xdiff = temp.x-input.x;
            double ydiff = temp.y-input.y;

            double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff)); //Distance between two particles, done right
            //printf("distance is %f \n", distance);
            
            if(distance == 0){
                continue; //If the distance is zero, the force is infinite, so we skip it
            }else{
                
            double force = G*temp.weight*input.weight/(distance*distance); //Force between two particles

            double acceleration = force/input.weight; //Acceleration of the particle

            double xc = xdiff/distance; //Cosine of the angle between the two particles
            double yc = ydiff/distance; //Sine of the angle between the two particles
            input.x_delta = input.x_delta + acceleration*xc;
            input.y_delta = input.y_delta + acceleration*yc;
            }
        } 
    }

    storage[x].x_delta = input.x_delta;
    storage[x].y_delta = input.y_delta;

    return;
}


int bruteForceUpdate(particle_storage& storage){

    size_t size = storage.getSize();


    for (int i = 0; i < size; i++)
    {
        gravtiationalForceUpdate(i, storage);
    }
    
    


    
    return 0;
}
