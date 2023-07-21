#pragma once
#include <vector>
#include <stdlib.h>
#define MY_DEBUG
template <typename T>
class grid
{
public:
    // Constructors
    grid(size_t width, size_t height) : width(width), height(height), data(nullptr)
    {
        data = new T[width * height];
        #ifdef MY_DEBUG
        printf("Construct %p \n", data);
        #endif
    };
    // grid(const grid& other) : width(other.width), height(other.height), data(other.data) {}
    ~grid()
    { // Destructor
        #ifdef MY_DEBUG
        printf("delete %p \n", data);
        #endif
        delete[] data;
    }
    grid(const grid &) = delete;
    grid &operator=(const grid &) = delete;

    grid(const grid &&);
    grid &operator=(grid<T> &&other) noexcept; // Move assignment operator

    T &operator[](int i)
    {
        if (i >= width * height || i < 0)
        {   

            printf("out of bounds %d \n", i);
        }
        return data[i];
    } // get value at index

    const T &operator[](int i) const
    {
        if (i >= width * height || i < 0)
        {
            printf("out of bounds %d \n", i);
        }
        return data[i];
    } // get value at index

    inline T *getData() { return data; }
    inline size_t getHeight() const { return height; }
    inline size_t getWidth() const { return width; }
    inline double& get(int x, int y){
        #ifdef MY_DEBUG
        if(x<0 || x>=width || y<0 || y>=height){
            printf("out of bounds x is %d, y is %d \n", x,y);
        }
        #endif
        return data[x+y*width];}
    inline const double& get(int x, int y) const{return data[x+y*width];}
    inline void setData(T* data){this->data=data;}
    // Copy assignment operator, deep copy
    grid<T> copy() const;
    
    inline void swap(grid<T>&other){
        if(width!=other.width || height!=other.height){
            printf("Grids are not the same size, cannot swap \n");
            return;
        }
        std::swap(data,other.data);
    }
private:
    T *data; // X and Y delta values
    size_t width, height;
};

template <typename T>
grid<T>::grid(const grid<T> &&other) : width(other.width), height(other.height), data(other.data)
{
    #ifdef MY_DEBUG
    printf("Move construct %p \n", data);
    #endif
}

template <typename T>
grid<T> &grid<T>::operator=(grid<T> &&other) noexcept
{
    if (this != &other)
    {
        width = other.width;
        height = other.height;
        data = other.data;
        other.data = nullptr;
        printf("Move assign %p \n", data);
    }
    return *this;
}

template <typename T>
grid<T> grid<T>::copy() const
{
    grid<T> new_field(width, height);
    size_t size = width * height;
    std::copy(data, data + size, new_field.data); // Uses the move assignment operator to move the data vector
    return new_field;
}
