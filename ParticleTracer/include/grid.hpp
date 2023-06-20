#pragma once
#include <vector>
#include <stdlib.h>
template <typename T>
class grid{
    public:
    //Constructors
    grid(size_t width, size_t height): width(width), height(height),data(width*height){

    };
    grid(const grid& other) : width(other.width), height(other.height), data(other.data) {}
    ~grid(){} //Destructor

    //grid(const grid &) = delete; THIS MIGHT BE BIG TROUBLE
    grid &operator=(const grid &) = delete;    
    

    grid &operator=(grid<T>&& other) noexcept; //Move assignment operator

    T& operator[](int i){return data[i];} //get value at index
    const T& operator[](int i) const {return data[i];} //get value at index
    
    inline std::vector<T>& getData(){return data;}
    inline size_t getHeight()const {return height;}
    inline size_t getWidth()const {return width;}
    inline void setData(T data){this->data=data;} 
    //Copy assignment operator, deep copy
    grid<T> copy() const;

    private:
    std::vector<T> data; //X and Y delta values
    size_t width, height;
};


template <typename T>
grid<T>& grid<T>::operator=(grid<T>&& other) noexcept{
    if(this != &other){
        width = other.width;
        height = other.height;
        data = std::move(other.data);
    }
    return *this;
}

template <typename T>
grid<T> grid<T>::copy() const {
    grid<T> new_field(width, height);
    new_field.data = data;  // Uses the move assignment operator to move the data vector

    return new_field;
}

