

#include <vector>
#include <stdlib.h>
#include <cmath>
const int width;
const int height;
size_t width, height;
std::vector<double> data;

// x1y2 ----- x2y2
//  |           |
//  |    x,y    |
//  |           |
// x1y1 ----- x2y1


double * bilinearInterpolateFields(double x, double y){
    //Corners for an point
    double x1 = floor(x);
    double x2 = ceil(x);
    double y1 = floor(y);
    double y2 = ceil(y);

    double divided = (x2-x1)*(y2-y1);
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
}
