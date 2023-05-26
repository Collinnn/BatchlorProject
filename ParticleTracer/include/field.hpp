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

    double* bilinearInterpolateFields(double x, double y);


    private:
    std::vector<double> data;
    size_t width, height;
};

template <typename T>
field<T>::field() : width(0),heigh(0),data(){}

template <typename T>
field<T>::field(size_t width_v, size_t height_v) : width(width_v), height(height_v), data().resize(width*height){}

template <typename T> //Error why?
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

//Should be moved to a separate file which handles the conversion from field to 
template <typename T>
double* field<T>::bilinearInterpolateFields(double x, double y){
    //Corners for an point
    double x1 = floor(x);
    double x2 = ceil(x);
    double y1 = floor(y);
    double y2 = ceil(y);

    double divided = (x2-x1)*(y2-y1);
    if(divided==0.0){
        return new double[4]{0.0,0.0,0.0,0.0}; 
    }
    //Distances from the corners
    double x2x = x2-x;
    double xx1 = x-x1;
    double y2y = y2-y;
    double yy1 = y-y1;
    //Interpolation weights
    double w11 = (x2x)*(y2y)/divided;
    double w12 = (x2x)*(yy1)/divided;
    double w21 = (xx1)*(y2y)/divided;
    double w22 = (xx1)*(yy1)/divided;
    return new double[4]{w11,w12,w21,w22};
}