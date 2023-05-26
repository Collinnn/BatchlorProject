#pragma once

#include "./particleTypes.hpp"
#include <algorithm>
#include <iterator>
#include <stdlib.h>

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
template <typename T>
class ParticlesTemplated
{
    ParticlesTemplated();
    ParticlesTemplated(size_t num_particles);
    ~ParticlesTemplated();

    ParticlesTemplated(ParticlesTemplated& other) = delete; //Copy constructor deleted
    ParticlesTemplated(ParticlesTemplated&& other) noexcept; //Move constructor
    ParticlesTemplated<T>& operator=(ParticlesTemplated<T>&&) noexcept; //Move assignment operator

    inline T& operator[](int i){return data_ptr[i];} //get value at index
    inline const T& operator[](int i) const {return data_ptr[i];} //get value at index

    inline bool is_empty() const {return data_ptr == nullptr;}

    void clear(); //Clears the data
    void swap(ParticlesTemplated<T>& other); //Swaps the data with another ParticlesTemplated<T>

    private:
        void free(); //Frees the data, is used internally in the hpp file

    private:
    T* data_ptr;
    //omp_target_alloc(particleArr, 10000*sizeof(particle2D), omp_get_default_device());
    // The above outcommented needs mapping to CPU
};

template <typename T>
ParticlesTemplated<T>::ParticlesTemplated() : data_ptr(nullptr){}

template <typename T>
ParticlesTemplated<T>::ParticlesTemplated(size_t num_particles) : data_ptr(nullptr){
    data_ptr = new T[num_particles];
};
template <typename T>
ParticlesTemplated<T>::~ParticlesTemplated(){
   if(!is_empty()){ // data_ptr != nullptr
       this->free();
   }
}
//other data_ptr is now new pointer and old is null

template <typename T>
ParticlesTemplated<T>::ParticlesTemplated(ParticlesTemplated<T>&& other) noexcept : data_ptr(other.data_ptr){
    other.data_ptr = nullptr;
}

template <typename T>
ParticlesTemplated<T>& ParticlesTemplated<T>::operator=(ParticlesTemplated<T>&& other) noexcept{
    if(this != &other){
        if(!is_empty()){
            this->free();
        }

        data_ptr = other.data_ptr;
        other.data_ptr = nullptr;
    }
    return *this;
}

template <typename T>
void ParticlesTemplated<T>::clear(){
    if(!is_empty()){
        this->free();
        m_ptr=nullptr;
    }
}

template <typename T>
void ParticlesTemplated<T>::swap(ParticlesTemplated<T>& other){
    std::swap(data_ptr, other.data_ptr);
}

template <typename T> 
void ParticlesTemplated<T>::free(){
    delete[] data_ptr;
}