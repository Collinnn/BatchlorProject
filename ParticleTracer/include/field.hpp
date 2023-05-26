#pragma once
#include <vector>
#include <stdlib.h>
template <typename T>
class field{
    public:
    //Constructors
    field();
    field(size_t width, size_t height);
    //field(size_t width, size_t height, std::vector<T> data);

    field(field& other) = delete; //Copy constructor deleted
    field(field&& other) noexcept; //Move constructor
    
    ~field(); //Destructor

    field& operator=(field<T>&& other) noexcept; //Move assignment operator

    T& operator[](int i){return data[i];} //get value at index
    const T& operator[](int i) const {return data[i];} //get value at index
    inline height()const {return height;}
    inline width()const {return width;}

    //Copy assignment operator, deep copy
    field<T> copy() const;

    double * bilinearInterpolateFields(double x, double y);


    private:
    std::vector<double> data;
    size_t width, height;
};

template <typename T>
field<T>::field() : width(0),heigh(0),data(){}

template <typename T>
field<T>::field(size_t width_v, size_t height_v) : width(width_v), height(height_v), data().resize(width*height){}

template <typename T>
field<T>::field(field<T>&& other) noexcept : width(other.width),height(other.height),data(std::move(other.data)){}

template <typename T>
field<T>::~field(){}

template <typename T>
field<T>& field<T>::operator=(field<T>&& other) noexcept{
    if(this != &other){
        width = other.width;
        height = other.height;
        data = std::move(other.data);
    }
    return *this;
}

template <typename T>
field<T> field<T>::copy() const{
    field<T> new_field(width,height);
    for(int i = 0; i < width*height; i++){
        new_field[i] = data[i];
    }
    return new_field;
}