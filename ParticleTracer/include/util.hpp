#pragma once

typedef struct
{
    double x;
    double y;
}vec2;

typedef struct
{
    double x;
    double y;
    double z;
}vec3;

typedef struct{
    size_t width,height;
    double physical_width, physical_height;
    double cell_width, cell_height;
} domain_info;

typedef struct range_t {
    int start_x, start_y;
    int end_x, end_y;
} range2D;
