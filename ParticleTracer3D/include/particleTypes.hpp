

#pragma once
#include <stdio.h>


typedef struct 
{
    double x; //Positions
    double y;
    double weight; // how much does it do against other particles
    double x_delta; // acceleration movement in a certain path
    double y_delta;
} particle2D;

typedef struct 
{
    double x; //Positions
    double y;
    double z;
    double weight; // how much does it do against other particles
    double x_delta; // acceleration movement in a certain path
    double y_delta;
    double z_delta;
} particle3D;

