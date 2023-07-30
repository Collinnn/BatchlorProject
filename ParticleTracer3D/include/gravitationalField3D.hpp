#pragma once
#include <stdio.h>
#include "./particleTypes.hpp"
#include "./grid.hpp"
#include <cmath>
#include "util.hpp"

class gravitationalField3D{
    
    public:
        //Constructor
        gravitationalField3D(domain_info domain): density_map((domain.width/domain.cell_width),(domain.height/domain.cell_height),(domain.depth/domain.cell_depth)), potential_map(domain.width/domain.cell_width,domain.height/domain.cell_height,domain.depth/domain.cell_depth){};
        //Destructor
        ~gravitationalField3D(){}; 

        gravitationalField3D(gravitationalField3D& other) = delete; //Copy constructor deleted
        gravitationalField3D &operator=(const gravitationalField3D&) = delete; //Copy assignment operator deleted

        grid<double>& getDensityMap(){return density_map;}
        grid<double>& getPotentialMap(){return potential_map;}

    private:
        grid<double> density_map;
        grid<double> potential_map; 
};

 

