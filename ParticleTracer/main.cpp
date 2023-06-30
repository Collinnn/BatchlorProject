
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <ctime> 
#include "./include/particleTypes.hpp"
#include "./include/particles.hpp"
#include "./include/gravitationalField2D.hpp"
#include "./include/interpolation.hpp"
#include "./include/grid.hpp"
#include "./include/bruteforce.hpp"
#include "./include/util.hpp"
#include "./include/io.hpp"

#include <cmath>
#include <tgmath.h>
#include <random>
#include <cassert>
#include <vector>
#include <omp.h>


int initArrays(domain_info domain, particle_storage &storage){
    //Use below in final version, debug is easier with a fixed seed
    //std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen32(0); //Standard mersenne_twister_engine seeded with rd
    std::uniform_real_distribution<double> dist1{0.1, 1.0}; 
    std::uniform_real_distribution<double> dist2{-0.1, 0.1};
    std::uniform_real_distribution<double> distx{5.0, domain.width-5.0};
    std::uniform_real_distribution<double> disty{5.0, domain.height-5.0};
    int size = domain.height*domain.width;

    for (int i = 0; i < size; i++)
    {            
        particle2D input; //here
        input.x = distx(gen32);
        input.y = disty(gen32);
        input.x_delta = dist2(gen32); //Random speed values
        input.y_delta = dist2(gen32); //Random speed values
        input.weight  = dist1(gen32);   //Random weight 0-1
        storage[i] = input;
        particle2D& particle = storage[i];
    }
    return 0;   
}


void moveParticles(domain_info domain, particle_storage &particle,bool& inbounds){
    int size = domain.height*domain.width;
    int num_steps = 3; //Number of steps to move particles
    for(int i = 0; i<size;i++){
        int x = particle[i].x_delta*num_steps;
        int y = particle[i].y_delta*num_steps;
        //Should nearly always succeed which leads to the pipeline not being emptied at failed if
        if(x<domain.width || x>0 || y<domain.height || y>0){ 
            particle[i].x += x;
            particle[i].y += y;
            continue; //skips rest of loop
        }
        inbounds = true;
        printf("out of bounds, x is %f, y is %f \n",x,y);
        return;
    }
}
void setGridToZero(grid<double>& grid){
    size_t width = grid.getWidth();
    size_t height = grid.getHeight();
    for(int x = 0; x<width;x++){
        for(int y = 0;y<height;y++){
            int index = y*width+x;
            grid[index] = 0;
        }
    }
}

void getDensity(domain_info domain,range2D& range,particle_storage &storage,gravitationalField2D &solver){
    double G = 6.67430e-11; // gravitational constant

    
    grid<double>& density = solver.getDensityMap();
    grid<double>& potential = solver.getPotentialMap();
    setGridToZero(density);
    setGridToZero(potential);
    
   
    double dx = domain.cell_width;
    double dy = domain.cell_height;
    int width = density.getWidth();
    int height = density.getHeight();
    int size = width*height;
    for(int i = 0; i<storage.getSize();i++){

        const particle2D& particle = storage[i];
        
        int cell_x = (int) (particle.x/dx)+range.start_x; 
        int cell_y = (int) (particle.y/dy)+range.start_y;
        int index = cell_y * width + cell_x;
        
        assert(index < size-1);
        if(index<size-1 && index>0){
            //Add the weight of the particles

            density[index] += particle.weight; //Starts at zero
            
        }
    }
    for(int x= range.start_x; x<range.end_x;x++){
        for(int y=range.start_y;y<range.end_y;y++){
            int index = y*width+x;
            //Right side of Poisson equation
            potential[index] = 4*M_PI*G*density[index];
        }
    }
 
}




double iterativeJacobi(domain_info domain,gravitationalField2D& field, size_t limit){
    int hh = (domain.cell_height*domain.cell_height)*(domain.cell_height*domain.cell_height);
    size_t width = field.getDensityMap().getWidth();
    size_t height = field.getDensityMap().getHeight();
    size_t size = height*width;
    
    grid<double>& density = field.getDensityMap();
    grid<double>& potential = field.getPotentialMap();
    grid<double> temp = std::move(density.copy());
    double error = 0;

    for(int i = 0; i<limit;i++){
        
        //Reduction is a shared memory operation
        //Collapse is a loop optimization to combine two loops 
        #pragma omp parallel for collapse(2) reduction(+:error)
        for(int x=1; x<width-1;x++){
            for(int y=1;y<height-1;y++){
                int index = y*width+x;
                density[index] = 0.25*(temp[index-1]+temp[index+1]+temp[index-width]+temp[index+width]+hh*potential[index]);
                error += density[index] - temp[index];
            }
        }
        error/= size;

        #pragma omp parallel for collapse(2) reduction(+:error)
        for(int x=1; x<width-1;x++){
            for(int y=1;y<height-1;y++){
                int index = y*width+x;
                temp[index] = 0.25*(density[index-1]+density[index+1]+density[index-width]+density[index+width]+hh*potential[index]);
                error+= density[index] - temp[index];
            }
           
        }

        error /= size;
        //Potential is zero.
    }
    return error;
}

void updateforce(domain_info domain, particle_storage& particles,gravitationalField2D& solver){
    grid<double>& potential = solver.getPotentialMap();
    int width = potential.getWidth();
    int height = potential.getHeight();
    double force;
    double distance,xdiff,ydiff,acceleration,xc,yc;
    for(int i = 0; i<particles.getSize();i++){
        // offset is 1 to account for the -1 to 1 grid
        int offset = 1;
        particle2D& particle = particles[i];
        int cell_x = (int) (particle.x/domain.physical_width)+offset; 
        int cell_y = (int) (particle.y/domain.physical_height)+offset;
        int index = cell_y * width + cell_x;
        
        assert(index<width*height);
        force = potential[index];

        xdiff = particle.x - cell_x*domain.cell_width;
        ydiff = particle.y - cell_y*domain.cell_height;

        distance = sqrt(xdiff*xdiff+ydiff*ydiff);
        acceleration = force/particle.weight;
        xc = xdiff/distance;
        yc = ydiff/distance;
        particle.x_delta += acceleration*xc;
        particle.y_delta += acceleration*yc;
    }
}

 void initRangeAndDomain(range2D& range,domain_info& domain){
    domain.width = 100; domain.height = 100;
    domain.physical_width = 10.0; domain.physical_height = 10.0;
    domain.cell_width = domain.physical_width/domain.width;
    domain.cell_height = domain.physical_height/domain.height;
    range.start_x = 1; range.start_y = 1;
    range.end_x = domain.width-1; range.end_y = domain.height-1;
 }

int main(){
    domain_info domain = {};
    range2D range = {};
    timeZone time = {};
    initRangeAndDomain(range,domain);
    printf("%d",omp_get_max_threads());
    return;

    printf("cell width is %f \n",domain.cell_width);
    printf("cell height is %f \n",domain.cell_height);
    printf("physical width is %f \n",domain.physical_width);
    printf("physical height is %f \n",domain.physical_height);
    printf("width is %d \n",domain.width);
    printf("height is %d \n",domain.height);
    particle_system particles(domain);
    particle_storage storage(10000000);
    gravitationalField2D solver(domain);

    bool bruteForce = false;
    bool inbounds = false;
    bool running = true;
    int max = 100; 
    int iteration = 0;//zeroes out the grids for resetting
    
    double error = 0;
    //timestep instead of iteration
    //Find max value of delta and use that to find the number of timesteps between simulation loop
    time.totaltime = omp_get_wtime();
    initArrays(domain,storage);
    setGridToZero(solver.getDensityMap());
    setGridToZero(solver.getPotentialMap());
    while(running && max>iteration){
        time.movetime = omp_get_wtime();
        
        moveParticles(domain,storage,inbounds);
        
        //Time calculations
        double moveTime = omp_get_wtime() - time.movetime;
        if(moveTime>time.maxmovetime){
            time.maxmovetime = moveTime;
        }else if (moveTime<time.minmovetime){
            time.minmovetime = moveTime;
        }
        time.movetime = moveTime;
        time.avgmovetime = (time.avgmovetime*iteration+time.movetime)/(iteration+1);
        
        
        if(bruteForce){

        }else{
            //Compute the density
            time.densitytime = omp_get_wtime();
            getDensity(domain,range,storage,solver);
            //Time calculations
            time.densitytime = omp_get_wtime() - time.densitytime;
            if(time.densitytime>time.maxdensitytime){
                time.maxdensitytime = time.densitytime;
            }else if (time.densitytime<time.mindensitytime){
                time.mindensitytime = time.densitytime;
            }

            time.iterativetime = omp_get_wtime();
            error = iterativeJacobi(domain,solver,100);
            time.iterativetime = omp_get_wtime() - time.iterativetime;
            printf("error: %f\n",error);
            if(time.iterativetime>time.maxiterativetime){
                time.maxiterativetime = time.iterativetime;
            }else if (time.iterativetime<time.miniterativetime){
                time.miniterativetime = time.iterativetime;
            }

            time.forcetime = omp_get_wtime();
            updateforce(domain,storage,solver);
            time.forcetime = omp_get_wtime() - time.forcetime;
            if(time.forcetime>time.maxforcetime){
                time.maxforcetime = time.forcetime;
            }else if (time.forcetime<time.minforcetime){
                time.minforcetime = time.forcetime;
            }

        }
        
        iteration++;
    }
    time.totaltime = omp_get_wtime() - time.totaltime;
    toCSVfile(time);
    printf("To PPM file\n");
    toPPMfile(domain,storage);
    //printf("Done\n");

    return 0;
}

