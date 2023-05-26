
#include "./include/particles.hpp"



particle2D* particles::particleArr[1000];

particle2D* particles::getParticleArray(){
    return particleArr;
}
particle2D particles::getParticleArrayValue(int i){
    return particleArr[i];
}

void particles::setParticleArrayValue(int i, particle2D value){
    particleArr[i] = value;
}