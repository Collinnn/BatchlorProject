

#pragma once
#include <stdio.h>


typedef struct 
{
    double x; //Postions
    double y;
    double weight; // how much does it do against other particles
    double x_delta; // accelertion movement in a certain path
    double y_delta;
} particle2D;

typedef struct 
{
    double x; //Postions
    double y;
    double z;
    double weight; // how much does it do against other particles
    double x_delta; // acceleartion movement in a certain path
    double y_delta;
    double z_delta;
} particle3D;

