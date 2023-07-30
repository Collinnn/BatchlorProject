
#include "./include/interpolation.hpp"
#include <vector>
#include <stdlib.h>
#include <cmath>
#include "./include/particleTypes.hpp"
#include "./include/grid.hpp"
#include "./include/util.hpp"
#include "./include/gravitationalField2D.hpp"
//#define MY_DEBUG

std::vector<double> data;

// x1y2 ----- x2y2
//  |           |
//  |    x,y    |
//  |           |
// x1y1 ----- x2y1

vec2 nearestNeighbor(particle2D p){
    double x = p.x;
    double y = p.y;
    vec2 output;
    double x1 = floor(x);
    double x2 = ceil(x);
    double y1 = floor(y);
    double y2 = ceil(y);


    if(x-x1<=x2-x){
        output.x = x1;
        if(y-y1<=y2-y){
            output.y = y1;
            return output;
        }else{
            output.y = y2;
            return output;
        }
    }
    output.x = x2;
    if(y-y1<=y2-y){
        output.y = y1;
        return output;
    }
    output.y = y2;
    return output;


}


double lerp(double v0,double v1,double t){
    //return v0 + t * (v1 - v0);
    return (1 - t) * v0 + t * v1; //More accurate
}
vec2 bilerp(vec2 a, vec2 b, vec2 c, vec2 d){
    double offset = 0.1;
    double abx = lerp(a.x,b.x,offset);
    double cdx = lerp(c.x,d.x,offset);
    double acy = lerp(a.y,c.y,offset);
    double bdy = lerp(b.y,d.y,offset);
    double abcdx = lerp(abx,cdx,offset);
    double abcdy = lerp(acy,bdy,offset);
    vec2 output;
    output.x = abcdx;
    output.y = abcdy;
    return output; 
}





/*
void* bilinearInterpolateFields(double x, double y, double weightArr[8]){ //LinearInterpolation

    //Corners for an point
    double x1 = floor(x); double x2 = ceil(x);
    double y1 = floor(y); double y2 = ceil(y);
    

    double divided = (x2-x1)*(y2-y1);

    if(divided==0.0){
        return;
    }
    int x1y1 = (int) x1+y1*field.getWidth();
    int x1y2 = (int) x1+y2*field.getWidth();
    int x2y1 = (int) x2+y1*field.getHeight();
    int x2y2 = (int) x2+y2*field.getHeight();


    //Distances from the corners
    double x2x = x2-x;
    double xx1 = x-x1;
    double y2y = y2-y;
    double yy1 = y-y1;
    //Interpolation weights
    double w11 = (x2x)*(y2y)/divided;
    double w12 = (x2x)*(yy1)/divided;
    double w21 = (xx1)*(y2y)/divided;
    double w22 = (xx1)*(yy1)/divided;
    //Interpolated field
    double weight = p.weight; double x_delta = p.x_delta; double y_delta = p.y_delta;

    //Update the fields
    field[x1y1].weight += weight*w11; field[x1y1].x_delta += x_delta*w11; field[x1y1].y_delta += y_delta*w11;
    field[x1y2].weight += weight*w12; field[x1y2].x_delta += x_delta*w12; field[x1y2].y_delta += y_delta*w12;
    field[x2y1].weight += weight*w21; field[x2y1].x_delta += x_delta*w21; field[x2y1].y_delta += y_delta*w21;
    field[x2y2].weight += weight*w22; field[x2y2].x_delta += x_delta*w22; field[x2y2].y_delta += y_delta*w22;


    return;
}
*/

double interpolationKernel(double s, double a){ //Needed in bicubic interpolation
    double sabs = abs(s);
    double sabs2 = sabs*sabs;
    if(sabs>=0.0 && sabs<=1.0){
        return (a+2)*(sabs2*sabs)-(a+3)*(sabs2)+1;
    }else if (sabs>1.0 && sabs<=2.0)
    {
        return a*(sabs2*sabs)-(5*a)*(sabs2)+(8*a)*(sabs)-4*a;
    }
    
}
