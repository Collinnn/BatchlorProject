#pragma once

#include "./particleTypes.hpp"
#include <algorithm>
#include <iterator>

class particles
{

private:
    static const int length = 10000;
    static particle2D particleArr[length];
    
    //omp_target_alloc(particleArr, 10000*sizeof(particle2D), omp_get_default_device());
    // The above outcommented needs mapping to CPU
public:
    static int getLength(){return length;}
    static particle2D* getParticleArray(){return particleArr;}
    static particle2D getParticleArrayValue(int i){return particleArr[i];}
    static void setParticleArrayValue(int i, particle2D& value){particleArr[i] = value;}
    static void setParticleArray(particle2D* value){for (int i = 0; i < length; i++){particleArr[i] = value[i];}}
    
};

//Allocates memory for the static array
particle2D particles::particleArr[length];




