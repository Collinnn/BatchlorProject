#pragma once
#include <stdio.h>
#include "./particleTypes.hpp"
#include "./grid.hpp"
#include <cmath>
#include "util.hpp"

class gravitationalField2D{
    
    public:
        //Constructors
        //The +2 is to make it 2 larger than needed, this makes the jacobi solver easier
        gravitationalField2D(domain_info domain): density_map((domain.width/domain.cell_width),(domain.height/domain.cell_height)), potential_map(domain.width/domain.cell_width,domain.height/domain.cell_height){};
        ~gravitationalField2D(){}; //Destructor

        gravitationalField2D(gravitationalField2D& other) = delete; //Copy constructor deleted
        gravitationalField2D &operator=(const gravitationalField2D&) = delete; //Copy assignment operator deleted

        grid<double>& getDensityMap(){return density_map;}
        grid<double>& getPotentialMap(){return potential_map;}
        //gravitationalField2D copy() const;

    private:
        grid<double> density_map;
        grid<double> potential_map; 
};

 

