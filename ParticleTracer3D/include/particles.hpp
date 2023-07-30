#pragma once

#include "./particleTypes.hpp"
#include "./grid.hpp"
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <memory>
#include "util.hpp"
#define MY_DEBUG

class particle_storage{ //this stores the particles
    public:
        particle_storage(size_t capacity){
            particles_ptr = new particle3D[capacity];
            #ifdef MY_DEBUG
            printf("Construct particle %p \n",particles_ptr);
            #endif
            size = capacity;
        } // num of particles
        ~particle_storage(){
            #ifdef MY_DEBUG
            printf("Deleted particle %p \n",particles_ptr);
            #endif
            delete[] particles_ptr; //array delete
        }

        //Delete copy constructor and assignment operator
        particle_storage(const particle_storage&) = delete;
        particle_storage &operator=(const particle_storage&) = delete;

        particle3D &operator[](size_t index){return particles_ptr[index];}
        const particle3D &operator[](size_t index) const{return particles_ptr[index];}
        inline particle3D* copy() const;

        size_t getSize() const {return size;}
    private:
        particle3D* particles_ptr;
        size_t size; // The number of particles in the array
};
particle3D* particle_storage::copy() const {
    particle3D* new_particles = new particle3D[size];
    std::copy(particles_ptr, particles_ptr + size, new_particles);
    return new_particles;
}

class particle_system{
    public:
        particle_system(domain_info,int amount) : storage(amount){}
        ~particle_system(){} //Works because the destructors are automatic
        inline particle_storage& getParticleStorage(){return storage;}
    private:
        particle_storage storage;
        domain_info domain;
};

