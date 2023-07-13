
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
#include <float.h>
//#define MY_DEBUG
// export OMP_NUM_THREADS= 4

int initParticles(domain_info& domain, particle_storage &storage){
    //Use below in final version, debug is easier with a fixed seed
    //std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen32(0); //Standard mersenne_twister_engine seeded with rd
    std::uniform_real_distribution<double> weight{0.1, 1.0}; 
    std::uniform_real_distribution<double> forces{-0.1, 0.1};
    std::uniform_real_distribution<double> distx{5.0, domain.width-5.0};
    std::uniform_real_distribution<double> disty{5.0, domain.height-5.0};
    
    size_t size = storage.getSize();

    for (int i = 0; i < size; i++)
    {   
        #ifdef MY_DEBUG
        assert(i>0&&i<size);
        #endif

        particle2D input; //here
        input.x = distx(gen32);
        input.y = disty(gen32);
        input.x_delta = forces(gen32); //Random speed values
        input.y_delta = forces(gen32); //Random speed values
        input.weight  = weight(gen32);   //Random weight 0-1

        storage[i] = input;
        particle2D& particle = storage[i]; 
        //printf("x is %f, y is %f, x_delta is %f, y_delta is %f, weight is %f \n",particle.x,particle.y,particle.x_delta,particle.y_delta,particle.weight);
    }
    return 0;   
}
void init2Particles(domain_info& domain,particle_storage& storage){
    storage[0].x = 50;
    storage[0].y = 50;
    storage[0].x_delta = 0;
    storage[0].y_delta = 0;
    storage[0].weight = 500;
    storage[1].x = 60;
    storage[1].y = 50;
    storage[1].x_delta = 0;
    storage[1].y_delta = 0.5;
    storage[1].weight = 1;
}

void moveParticles(domain_info& domain, particle_storage& particle,bool& outbounds,double& steps){
    int size = particle.getSize();
    
    for(int i = 0; i<size;i++){
        double x_delta = particle[i].x_delta*steps;
        double y_delta = particle[i].y_delta*steps;

        //Should nearly always succeed which leads to the pipeline not being emptied at failed if
        if(x_delta<domain.width || x_delta>=0 || y_delta<domain.height || y_delta>=0){
            
            particle[i].x += x_delta;
            particle[i].y += y_delta;
            continue; //skips rest of loop
        }
        outbounds = true;
        printf("out of bounds, x is %f, y is %f, index is %d \n",x_delta,y_delta,i);
        //return;
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

void getDensity(domain_info& domain,range2D& range,particle_storage& storage,gravitationalField2D& solver){
    double G = 6.67430e-11; // gravitational constant

    
    grid<double>& density = solver.getDensityMap();
    grid<double>& f = solver.getPotentialMap();
    setGridToZero(density);
    setGridToZero(f);

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
        if(index<size-1 && index>0){
            //Add the weight of the particles
            density[index] += particle.weight; //Starts at zero
        }
        else{
            
        }
    }
    int kx =2*M_PI;
    int ky =4*M_PI;

    for(int x= range.start_x; x<range.end_x;x++){
        for(int y=range.start_y;y<range.end_y;y++){
            int index = y*width+x;
            //Right side of Poisson equation
            f[index] = kx*kx*ky*ky*sin(kx*x)*sin(ky*y); //analytical solution
            //f[index] = 4*M_PI*G*density[index];
        }
    }
 
}




double poissonSolver(grid<double>& u,grid<double>& u_old, const grid<double>& f,double hh, size_t limit,range2D& range,double error){
    size_t width = u.getWidth();
    size_t height = u.getHeight();
    size_t size = height*width;


    for(int i = 0; i<limit;i++){
        
        //Reduction is a shared memory operation with the + saying after the loop add
        //Collapse is a loop optimization to combine two loops 
        #pragma omp parallel for collapse(2) reduction(+:error)
        for(int x=range.start_x; x<range.end_x;x++){
            for(int y=range.start_y;y<range.end_y;y++){
                int index = y*width+x;
                u[index] = 0.25*(u_old[index-1]+u_old[index+1]+u_old[index-width]+u_old[index+width]+hh*f[index]);
                
                if(i%5!=0){
                    continue;
                }else{
                    error += abs(u[index] - u_old[index]);
                }
                
            }
        }
        if(i%5!=0){
            continue;
        }else{
            error/= size;
        }
        
        #pragma omp parallel for collapse(2) reduction(+:error)
        for(int x=1; x<width-1;x++){
            for(int y=1;y<height-1;y++){
                int index = y*width+x;
                u_old[index] = 0.25*(u[index-1]+u[index+1]+u[index-width]+u[index+width]+hh*f[index]);
                error+= u[index] - u_old[index];
                if(i%5!=0){
                  continue;
                }else{
                    error += abs(u[index] - u_old[index]);
                }
            }
           
        }
    }
    return error;
}

double iterativeJacobi(domain_info& domain,gravitationalField2D& field, size_t limit,range2D& range,double error){
    int hh = (domain.cell_height*domain.cell_height)*(domain.cell_height*domain.cell_height);    
    grid<double>& u = field.getDensityMap();
    grid<double>& f = field.getPotentialMap();
    grid<double> u_old = std::move(u.copy());

    error = poissonSolver(u,u_old,f,hh,limit,range,error);
    return error;
}

void updateforce(domain_info& domain, particle_storage& particles,gravitationalField2D& solver){
    grid<double>& potential = solver.getPotentialMap();
    int width = potential.getWidth();
    int height = potential.getHeight();
    int size = width*height;
    double force;
    double distance,xdiff,ydiff,acceleration,xc,yc;
    size_t particle_size = particles.getSize();
    int offset = 1; //TODO: make this a parameter
    for(int i = 0; i<particle_size;i++){
        // offset is 1 to account for the -1 to 1 grid
        particle2D& particle = particles[i];
        int cell_x = (int) (particle.x/domain.cell_width)+offset; 
        int cell_y = (int) (particle.y/domain.cell_height)+offset;
        int index = cell_y * width + cell_x;

        if(index <size-1 && index>0){
            force = potential[index];

            xdiff = particle.x - cell_x*domain.cell_width;
            ydiff = particle.y - cell_y*domain.cell_height;

            if(xdiff != 0 || ydiff != 0){
                distance = sqrt(xdiff*xdiff+ydiff*ydiff);

                acceleration = force/particle.weight;



                xc = xdiff/distance;
                yc = ydiff/distance;

                particle.x_delta += acceleration*xc;
                particle.y_delta += acceleration*yc;


            }
        }
    }
}

void initRangeAndDomain(range2D& range,domain_info& domain){
    domain.width = 100; domain.height = 100;
    domain.physical_width = 10.0; domain.physical_height = 10.0;
    domain.cell_width = domain.physical_width/domain.width;
    domain.cell_height = domain.physical_height/domain.height;
    range.start_x = 1; range.start_y = 1;
    range.end_x = (domain.width/domain.cell_width)-1; range.end_y = (domain.height/domain.cell_height)-1;
 }
void initTime(timeZone& time){
    time.minmovetime = DBL_MAX;
    time.mindensitytime = DBL_MAX;
    time.miniterativetime = DBL_MAX;
    time.minforcetime = DBL_MAX;
}
void printDomainInfo(domain_info& domain){
    printf("cell width is %f \n",domain.cell_width);
    printf("cell height is %f \n",domain.cell_height);
    printf("physical width is %f \n",domain.physical_width);
    printf("physical height is %f \n",domain.physical_height);
    printf("width is %d \n",domain.width);
    printf("height is %d \n",domain.height);
}

int main(){
    domain_info domain = {};
    range2D range = {};
    timeZone time = {};
    initTime(time);
    initRangeAndDomain(range,domain);
    printf("Max number of threads is %d \n",omp_get_max_threads());
    
    
    printDomainInfo(domain);

    int amount = 2;
    //particle_system particles(domain,amount);
    particle_storage storage(amount);
    gravitationalField2D solver(domain);

    bool bruteForce = false;
    bool outbounds = false;
    bool running = true;
    int max = 10; 
    int iteration = 0;
    double steps = 0.01;
    double error = 0;
    int iter = 0;
    //timestep instead of iteration
    //Find max value of delta and use that to find the number of timesteps between simulation loop
    
    time.totaltime = omp_get_wtime();
    
    initParticles(domain,storage);
    init2Particles(domain,storage);
    setGridToZero(solver.getDensityMap());
    setGridToZero(solver.getPotentialMap());

    toPPMfile(domain,storage,iter);
    while(running && max-1>iteration){
        time.movetime = omp_get_wtime();
        
        moveParticles(domain,storage,outbounds,steps);
        toPPMfile(domain,storage,iter);
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
            bruteForceUpdate(storage);
        }else{
            printf("getting density\n");
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
            error = iterativeJacobi(domain,solver,100,range,error);
            time.iterativetime = omp_get_wtime() - time.iterativetime;
            printf("error: %f\n",error);
            if(time.iterativetime>time.maxiterativetime){
                time.maxiterativetime = time.iterativetime;
            }else if (time.iterativetime<time.miniterativetime){
                time.miniterativetime = time.iterativetime;
            }

            time.forcetime = omp_get_wtime();
            printf("updating force\n");
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
    
    toPPMfile(domain,storage,iter);
    //printf("Done\n");

    return 0;
}

