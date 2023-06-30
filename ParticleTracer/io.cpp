#pragma once
#include <iostream>
#include <fstream>
#include "./include/io.hpp"
#include "./include/particles.hpp"
#include "./include/util.hpp"
#include <cassert>

//Takes the global particle array and prints it to a ppm file
int toPPMfile(domain_info domain, particle_storage &storage){
    int height = domain.height;
    int width = domain.width;
    std::remove("./Output/output.ppm");
    std::ofstream fout("./Output/output.ppm");
    if(fout.fail()){
        std::cout << "Error opening file" << std::endl;
        return -1;
    }

    fout << "P3\n";
    fout << height << " " << width << "\n";
    fout << "255\n";
    
    int x,y;
    int Arr[height*width];
    int size= storage.getSize();
    //Adds together how many particles are in each pixel
    for (int i = 0; i < size; i++)
    {
        x = storage[i].x;
        y = storage[i].y;
        int index = y*width+x;
        assert(index<size);
        Arr[index] += 1;
       
    }
    printf("size is %d \n", size);
    printf("Done adding\n");
    //Prints the values of pixels to the file
    for (int i = 0; i < height*width; i++)
    {
        //printf("%d %d %d ", (int)Arr[i]*10, (int)Arr[i]*10, (int)Arr[i]*10);
        fout << (int)Arr[i]*10 << " " << (int)Arr[i]*10 << " " << (int)Arr[i]*10 << "  ";
    }
    fout.close();
    
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
    printf("Done writing to csv\n");
    fout.close();
}

int printArr(domain_info domain, particle_storage &storage){
    int size = domain.height*domain.width;
    for(int i = 0; i<size;i++){
        printf("x: %f, y: %f, x_delta: %f, y_delta: %f, weight: %f \n",
        storage[i].x, storage[i].y, storage[i].x_delta, storage[i].y_delta, storage[i].weight);
    }
    return 0;  
}