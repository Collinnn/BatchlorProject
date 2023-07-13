#pragma once
#include <iostream>
#include <fstream>
#include "./include/io.hpp"
#include "./include/particles.hpp"
#include "./include/util.hpp"
#include <cassert>
#include <algorithm>

//Takes the global particle array and prints it to a ppm file
int toPPMfile(domain_info& domain, particle_storage& storage,int& iter){
    int height = domain.height;
    int width = domain.width;
    int domainsize = height*width;
    std::string filename = "./Output/output"+std::to_string(iter)+".ppm";

    std::ofstream fout(filename.c_str());
    if(fout.fail()){
        std::cout << "Error opening file" << std::endl;
        return -1;
    }

    fout << "P3\n";
    fout << height << " " << width << "\n";
    fout << "255\n";
    
    int x,y;
    int Arr[domainsize];
    //Not compile time constant or constant expression
    std::fill(Arr, Arr + domainsize, 0);

    size_t size= storage.getSize();
    //Adds together how many particles are in each pixel
    for (size_t i = 0; i < size; i++)
    {
        x = storage[i].x;
        y = storage[i].y;

        if(x<0 || x>width || y<0 || y>height){
            continue;
        }
        //printf("x is %d, y is %d, i is %d \n", x, y, i);
        int index = y*width+x;
        if(index<domainsize-1 && index>=0){
            Arr[index] += 1;
            
        }else{
            //printf("index is %d \n", index);
            //printf("x,y is %d,%d \n", x,y);
        }
        
       
    }
    
    //Prints the values of pixels to the file
    for (int i = 0; i < domainsize; i++)
    {
        //printf("%d %d %d ", (int)Arr[i]*10, (int)Arr[i]*10, (int)Arr[i]*10);
        fout << (int)Arr[i]*200 << " " << (int)Arr[i]*200 << " " << (int)Arr[i]*200 << "  ";
    }
    fout.close();
    iter++;
    return 0;
}

void toCSVfile(timeZone& time){
    std::remove("./Output/output.csv");
    std::ofstream fout("./Output/output.csv");
    if(fout.fail()){
        std::cout << "Error opening file" << std::endl;
        return;
    }
    fout << "Time, AverageTime,MaxTime,MinTime\n";
    fout << time.totaltime << "\n";
    fout << time.movetime << "," << time.avgmovetime << "," << time.maxmovetime << "," << time.minmovetime << "\n";
    fout << time.densitytime << "," << time.avgdensitytime << "," << time.maxdensitytime << "," << time.mindensitytime << "\n";
    fout << time.iterativetime << "," << time.avgiterativetime << "," << time.maxiterativetime << "," << time.miniterativetime << "\n";
    fout << time.forcetime << "," << time.avgforcetime << "," << time.maxforcetime << "," << time.minforcetime << "\n";
    
    fout.close();
}

int printArr(domain_info& domain, particle_storage &storage){
    int size = domain.height*domain.width;
    for(int i = 0; i<size;i++){
        printf("x: %f, y: %f, x_delta: %f, y_delta: %f, weight: %f \n",
        storage[i].x, storage[i].y, storage[i].x_delta, storage[i].y_delta, storage[i].weight);
    }
    return 0;  
}