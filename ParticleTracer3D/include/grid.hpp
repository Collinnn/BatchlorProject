#pragma once
#include <vector>
#include <stdlib.h>
#define MY_DEBUG
template <typename T>
class grid
{
private:
    T *data; 
    size_t width, height, depth;
public:
    // Constructors
    grid(size_t width, size_t height,size_t depth) : width(width), height(height),depth(depth), data(nullptr)
    {
        data = new T[width * height * depth];
        #ifdef MY_DEBUG
        printf("Construct grid %p \n", data);
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
        if (i >= width * height * depth || i < 0)
        {   

            printf("out of bounds %d \n", i);
        }
        return data[i];
    } // get value at index

    const T &operator[](int i) const
    {
        if (i >= width * height * depth || i < 0)
        {
            printf("out of bounds %d \n", i);
        }
        return data[i];
    } // get value at index

    inline T *getData() { return data; }
    inline size_t getHeight() const { return height; }
    inline size_t getWidth() const  { return width;  }
    inline size_t getDepth() const  { return depth;  }

    inline double& get(int x, int y,int z){
        #ifdef MY_DEBUG
        if(x<0 || x>=width || y<0 || y>=height || z<0 || z>=depth){
            printf("out of bounds x is %d, y is %d, z is %d \n",x,y,z);
        }
        #endif
        return data[x+y*width+z*width*height];
    }

    inline const double& get(int x, int y,int z) const{return data[x+y*width+z*width*height];}
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
};



template <typename T>
grid<T>::grid(const grid<T> &&other) : width(other.width), height(other.height),depth(other.depth), data(other.data)
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
        depth = other.depth;
        data = other.data;
        other.data = nullptr;
        printf("Move assign %p \n", data);
    }
    return *this;
}

template <typename T>
grid<T> grid<T>::copy() const
{
    grid<T> new_field(width, height,depth);
    size_t size = width * height;
    std::copy(data, data + size, new_field.data); // Uses the move assignment operator to move the data vector
    return new_field;
}
