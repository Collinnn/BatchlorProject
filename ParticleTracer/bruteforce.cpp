#include <stdio.h>
#include "./include/particleTypes.hpp"
#include "./include/particles.hpp"
#define MY_DEBUG
double G = 6.67430e-11; //Gravitational constant


void gravtiationalForceUpdate(int x,particle_storage& storage){
    particle2D input = storage[x];

    double x_delta = input.x_delta;
    double y_delta = input.y_delta;
    int size = storage.getSize();
    for (int i = 0; i < size; i++){
        particle2D temp = storage[i];

        if(input.x == temp.x && input.y == temp.y){
            continue;
        }else{
            double xdiff = input.x-temp.x;
            double ydiff = input.y-temp.y;

            double distance = sqrt(xdiff*xdiff+ydiff*ydiff); //Distance between two particles, done right
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

    storage[x].x_delta = x_delta;
    storage[x].y_delta = y_delta;

}

int updateToNewCordinates(particle_storage& storage,int steps){
    size_t size = storage.getSize();
    //handles invalid steps
    if(steps<=0){
       steps =1;
    }
    for (int i = 0; i < size; i++)
    {
        storage[i].x += storage[i].x_delta*steps;
        storage[i].y += storage[i].y_delta*steps;

    }
    return 0;
}


int bruteForceUpdate(particle_storage& storage){
    size_t size = storage.getSize();
    particle_storage tempArr(size);
    for (int i = 0; i < size; i++)
    {
        tempArr[i] = storage[i];
    }
    for (int i = 0; i < size; i++)
    {
        gravtiationalForceUpdate(i, tempArr);
    }
    
    updateToNewCordinates(tempArr,1);

    for (int i = 0; i < size; i++)
    {
        storage[i] = tempArr[i];
    }
    tempArr.~particle_storage(); //Delete the temporary array 
    return 0;
}
