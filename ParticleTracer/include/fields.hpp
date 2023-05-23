#pragma once
#include <vector>
#include <stdlib.h>
class fields{
    private:
    std::vector<double> data;
    size_t width, height;

    double * bilinearInterpolateFields(double x, double y);
    double FEMFields();

};