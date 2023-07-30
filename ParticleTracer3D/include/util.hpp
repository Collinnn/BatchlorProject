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
    size_t width,height,depth;
    double physical_width, physical_height,physical_depth;
    double cell_width, cell_height,cell_depth;
} domain_info;

typedef struct{
    int start_x, start_y;
    int end_x, end_y;
} range2D;

typedef struct{
    double totaltime;
    double movetime, avgmovetime, maxmovetime,minmovetime;
    double densitytime, avgdensitytime, maxdensitytime, mindensitytime;
    double iterativetime, avgiterativetime, maxiterativetime, miniterativetime;
    double forcetime, avgforcetime, maxforcetime, minforcetime;
} timeZone;