
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <ctime> 
#include "./include/particleTypes.hpp"
#include "./include/particles.hpp"
#include "./include/gravitationalField3D.hpp"
#include "./include/interpolation.hpp"
#include "./include/grid.hpp"
#include "./include/bruteforce.hpp"
#include "./include/util.hpp"
#include "./include/io.hpp"
#include "./include/poissonSolver.hpp"
#include <cmath>
#include <tgmath.h>
#include <random>
#include <cassert>
#include <vector>
#include <omp.h>
#include <float.h>
#define MY_DEBUG
// export OMP_NUM_THREADS= 4

int initParticles(domain_info& domain, particle_storage &storage){
    //Use below in final version, debug is easier with a fixed seed
    //std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen32(0); //Standard mersenne_twister_engine seeded with rd
    std::uniform_real_distribution<double> weight{0.1, 1.0}; 
    std::uniform_real_distribution<double> forces{-0.1, 0.1};
    std::uniform_real_distribution<double> distx{5.0, domain.width-5.0};
    std::uniform_real_distribution<double> disty{5.0, domain.height-5.0};
    std::uniform_real_distribution<double> distz{5.0, domain.depth-5.0};
    
    size_t size = storage.getSize();

    for (int i = 0; i < size; i++)
    {   
        particle3D input; //here
        input.x = distx(gen32);
        input.y = disty(gen32);
        input.z = distz(gen32);
        input.x_delta = forces(gen32); //Random speed values
        input.y_delta = forces(gen32); //Random speed values
        input.z_delta = forces(gen32); //Random speed values
        input.weight  = weight(gen32);   //Random weight 0-1

        storage[i] = input;
        particle3D& particle = storage[i]; 
    }
    return 0;   
}
void init2Particles(particle_storage& storage){
    storage[0].x = 50;
    storage[0].y = 50;
    storage[0].x_delta = 0;
    storage[0].y_delta = 0;
    storage[0].weight = 5000000000000;
    storage[1].x = 60;
    storage[1].y = 50;
    storage[1].x_delta = 0;
    storage[1].y_delta = 0.5;
    storage[1].weight = 1;
}

void moveParticles(domain_info& domain, particle_storage& particle,bool& outbounds,double& steps){
    int size = particle.getSize();
    int width = (int)domain.width/domain.cell_width;
    int height = (int)domain.height/domain.cell_height;

    for(int i = 0; i<size;i++){
        double x_delta = particle[i].x_delta*steps;
        double y_delta = particle[i].y_delta*steps;
        double x = particle[i].x+x_delta;
        double y = particle[i].y+y_delta;

        //Should nearly always succeed which leads to the pipeline not being emptied at failed if
        if(x<width || x>=0 || y<height || y>=0){
            particle[i].x = x;
            particle[i].y = y;
            continue; //skips rest of loop
        }
        outbounds = true;
        printf("out of bounds, x is %f, y is %f, index is %d \n",x_delta,y_delta,i);
    }
}
void setGridToZero(grid<double>& grid){
    #ifdef MY_DEBUG
    printf("setGridToZero start \n");
    #endif
    size_t width = grid.getWidth();
    size_t height = grid.getHeight();
    size_t depth = grid.getDepth();
    for(int x = 0; x<width;x++){
        for(int y = 0;y<height;y++){
            for(int z = 0; z<depth;z++){
                grid.get(x,y,z) = 0.0;
            }
        }
    }
    #ifdef MY_DEBUG
    printf("setGridToZero end \n");
    #endif
}

void getDensity(domain_info& domain,poissonSolver& pSolver,particle_storage& storage,gravitationalField3D& Field3D){
    #ifdef MY_DEBUG
    printf("getDensity start \n");
    #endif
    double G = 6.67430e-11; // gravitational constant

    grid<double>& density = Field3D.getDensityMap();
    grid<double>& potential = Field3D.getPotentialMap();
    setGridToZero(density);
    setGridToZero(potential);

    double dx = domain.cell_width;
    double dy = domain.cell_height;
    double dz = domain.cell_depth;
    int width = density.getWidth();
    int height = density.getHeight();
    int depth = density.getDepth();

    for(int i = 0; i<storage.getSize();i++){

        const particle3D& particle = storage[i];

        int cell_x = (int) (particle.x/dx); 
        int cell_y = (int) (particle.y/dy);
        int cell_z = (int) (particle.z/dz);
        if(width>cell_x && height>cell_y && cell_x>=0 && cell_y>=0){
            //Add the weight of the particles
            density.get(cell_x,cell_y,cell_z) += particle.weight; //Starts at zero
        }

    }
    int kx =2*M_PI;
    int ky =4*M_PI;
    int kz =6*M_PI;
    int kxx= kx*kx;
    int kyy= ky*ky;
    int kzz= kz*kz;
    #ifdef MY_DEBUG
    printf("looping over rhs\n");
    #endif
    for(int x= 0; x<width;x++){
        for(int y=0;y<height;y++){
            for(int z=0;z<depth;z++){
                //Right side of Poisson equation
                pSolver.getF(x,y,z) = -(kxx + kyy + kzz)*(sin(kx*x)*sin(ky*y)*sin(kz*z)); //analytical solution
                //pSolver.getF(x,y,z) = 4*M_PI*G*density.get(x,y,z); 
            }

        }
    }
    #ifdef MY_DEBUG
    printf("getDensity end \n");
    #endif
}



double iterativeJacobi(domain_info& domain,gravitationalField3D& field,poissonSolver& pSolver,int& iterations,double& tolerance){
    int hh = (domain.cell_height*domain.cell_height)*(domain.cell_height*domain.cell_height)*(domain.cell_height*domain.cell_height);    
    grid<double>& density = field.getDensityMap();
    grid<double>& potential = field.getPotentialMap();
    int width = (int)domain.width/domain.cell_width;
    int height = (int)domain.height/domain.cell_height;
    int depth = (int)domain.depth/domain.cell_depth;

    double error = pSolver.solve(iterations,tolerance,hh);
    printf("error is %f \n",error);
    return error;

    for(int x = 0; x<width;x++){
        for(int y = 0;y<height;y++){
            for(int z=0;z<depth;z++){
                potential.get(x,y,z) = pSolver.getU(x,y,z);
            }
        }
    }


    return error;
}

void updateforce(domain_info& domain, particle_storage& particles,gravitationalField3D& Field3D){
    #ifdef MY_DEBUG
    printf("updateforce start\n");
    #endif
    grid<double>& potential = Field3D.getPotentialMap();
    int width = potential.getWidth();
    int height = potential.getHeight();
    double forcex,forcey,forcez;
    size_t particle_size = particles.getSize();
    
    #ifdef MY_DEBUGs
    for(int i=0;i<size;i++){
        if(potential[i]>0){
            printf("potential is %f \n",potential[i]);
        }
        
    }
    #endif
    for(int i = 0; i<particle_size;i++){
        // offset is 1 to account for the -1 to 1 grid
        particle3D& particle = particles[i];
        int cell_x = (int) (particle.x/domain.cell_width); 
        int cell_y = (int) (particle.y/domain.cell_height);
        int cell_z = (int) (particle.z/domain.cell_depth);
        if(cell_x<width && cell_y<height && cell_x>0 && cell_y>0){
            forcex = potential.get(cell_x+1,cell_y,cell_z)-potential.get(cell_x-1,cell_y,cell_z);
            forcey = potential.get(cell_x,cell_y+1,cell_z)-potential.get(cell_x,cell_y-1,cell_z);
            forcez = potential.get(cell_x,cell_y,cell_z+1)-potential.get(cell_x,cell_y,cell_z-1);
            printf("force is %f,%f \n",forcex,forcey);

            
            particle.x_delta += forcex;
            particle.y_delta += forcey;
            particle.z_delta += forcez;
        }
    }

    #ifdef MY_DEBUGs
    printf("updateforce end\n");
    #endif
}

void initDomain(domain_info& domain){
    domain.width = 100; domain.height = 100, domain.depth = 100;
    domain.physical_width = 100.0; domain.physical_height = 100.0, domain.physical_depth = 100.0;
    domain.cell_width = domain.physical_width/domain.width;
    domain.cell_height = domain.physical_height/domain.height;
    domain.cell_depth = domain.physical_depth/domain.depth;
}

void initTime(timeZone& time){
    time.minmovetime = INFINITY;
    time.mindensitytime = INFINITY;
    time.miniterativetime = INFINITY;
    time.minforcetime = INFINITY;
}

void printDomainInfo(domain_info& domain){
    printf("cell width is %f \n",domain.cell_width);
    printf("cell height is %f \n",domain.cell_height);
    printf("cell depth is %f \n",domain.cell_depth);
    printf("physical width is %f \n",domain.physical_width);
    printf("physical height is %f \n",domain.physical_height);
    printf("physical depth is %f \n",domain.physical_depth);
    printf("width is %d \n",domain.width);
    printf("height is %d \n",domain.height);
    printf("depth is %d \n",domain.depth);
}

int main(){

    //Inititalize variables
    domain_info domain = {};
    timeZone time = {};
    initTime(time);
    initDomain(domain);

    printf("Max number of threads is %d \n",omp_get_max_threads());
    
    printDomainInfo(domain);
    
    int amount = 0;
    particle_storage storage(amount);
    gravitationalField3D Field3D(domain);
    poissonSolver poissonSolver(domain);


    //Variables for simulation
    bool bruteForce = false;
    bool outbounds = false;
    bool running = true;
    int max = 1000; 
    int iteration = 0;
    int PoissonIterations = 0;
    int iter = 0;
    double steps = 0.0001;
    double tolerance = 0.00001;
    double error = 0;


    //timestep instead of iteration
    //Find max value of delta and use that to find the number of timesteps between simulation loop
    
    time.totaltime = omp_get_wtime();
    
    //initParticles(domain,storage);
    //init2Particles(storage);


    setGridToZero(Field3D.getDensityMap());
    setGridToZero(Field3D.getPotentialMap());

    toPPMfile(domain,storage,iter);
    while(running && max-1>iteration){
        time.movetime = omp_get_wtime();
        
        moveParticles(domain,storage,outbounds,steps);
        toPPMfile(domain,storage,iter);
        //Time calculations
        time.movetime = omp_get_wtime() - time.movetime;
        time.avgmovetime = (time.avgmovetime*iteration+time.movetime)/(iteration+1);
        if(time.movetime>time.maxmovetime){
            time.maxmovetime = time.movetime;
        }else if (time.movetime<time.minmovetime){
            time.minmovetime = time.movetime;
        }

        if(bruteForce){
            bruteForceUpdate(storage);
        }else{
            printf("getting density\n");
            //Compute the density
            time.densitytime = omp_get_wtime();
            getDensity(domain,poissonSolver,storage,Field3D);
            //Time calculations
            time.densitytime = omp_get_wtime() - time.densitytime;
            time.avgdensitytime = (time.avgdensitytime*iteration+time.densitytime)/(iteration+1);
            if(time.densitytime>time.maxdensitytime){
                time.maxdensitytime = time.densitytime;
            }else if (time.densitytime<time.mindensitytime){
                time.mindensitytime = time.densitytime;
            }

            time.iterativetime = omp_get_wtime();
            error = iterativeJacobi(domain,Field3D,poissonSolver,PoissonIterations,tolerance);
            time.iterativetime = omp_get_wtime() - time.iterativetime;
            time.avgiterativetime = (time.avgiterativetime*iteration+time.iterativetime)/(iteration+1);
            printf("error: %f\n",error);
            printf("iterations: %d\n",PoissonIterations);
            return;
            if(time.iterativetime>time.maxiterativetime){
                time.maxiterativetime = time.iterativetime;
            }else if (time.iterativetime<time.miniterativetime){
                time.miniterativetime = time.iterativetime;
            }

            time.forcetime = omp_get_wtime();
            updateforce(domain,storage,Field3D);
            time.forcetime = omp_get_wtime() - time.forcetime;
            time.avgforcetime = (time.avgforcetime*iteration+time.forcetime)/(iteration+1);
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


    return 0;
}

