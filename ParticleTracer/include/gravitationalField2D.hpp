#pragma once
#include <stdio.h>
#include "./particleTypes.hpp"
#include "./grid.hpp"
#include <cmath>
#include "util.hpp"

class gravitationalField2D{
    
    public:
        //Constructor
        gravitationalField2D(domain_info domain): density_map((domain.width/domain.cell_width),(domain.height/domain.cell_height)), potential_map(domain.width/domain.cell_width,domain.height/domain.cell_height){};
        //Destructor
        ~gravitationalField2D(){}; 

        gravitationalField2D(gravitationalField2D& other) = delete; //Copy constructor deleted
        gravitationalField2D &operator=(const gravitationalField2D&) = delete; //Copy assignment operator deleted

        grid<double>& getDensityMap(){return density_map;}
        grid<double>& getPotentialMap(){return potential_map;}

    private:
        grid<double> density_map;
        grid<double> potential_map; 
};

 

