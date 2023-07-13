#pragma once

#include "./particleTypes.hpp"
#include "./grid.hpp"
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <memory>
#include "util.hpp"


class particle_storage{ //this stores the particles
    public:
        particle_storage(size_t capacity){
            particles_ptr = new particle2D[capacity];
            size = capacity;
        } // num of particles
        ~particle_storage(){
            delete[] particles_ptr; //array delete
        }

        //Delete copy constructor and assignment operator
        particle_storage(const particle_storage&) = delete;
        particle_storage &operator=(const particle_storage&) = delete;

        particle2D &operator[](size_t index){return particles_ptr[index];}
        const particle2D &operator[](size_t index) const{return particles_ptr[index];}
        inline particle2D* copy() const;

        size_t getSize() const {return size;}
    private:
        particle2D* particles_ptr;
        size_t size; // The number of particles in the array
};
particle2D* particle_storage::copy() const {
    particle2D* new_particles = new particle2D[size];
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

