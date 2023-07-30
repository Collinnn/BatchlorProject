#pragma once
#include <stdlib.h>
#include "grid.hpp"
#include "util.hpp"
#include <omp.h>
class poissonSolver
{
private:
    grid<double> f;
    grid<double> u;
    grid<double> u_old;
public:
    poissonSolver(domain_info domain) : f(domain.width/domain.cell_width,domain.height/domain.cell_height,domain.depth/domain.cell_depth), u((domain.width/domain.cell_width)+2,(domain.height/domain.cell_height)+2,(domain.depth/domain.cell_depth)+2), u_old((domain.width/domain.cell_width)+2,(domain.height/domain.cell_height)+2,(domain.depth/domain.cell_depth)+2){
        printf("construct %p poissonSolver \n",this);
    };
    ~poissonSolver();
    poissonSolver(const poissonSolver &) = delete;            // Copy constructor deleted
    poissonSolver &operator=(const poissonSolver &) = delete; // Copy assignment operator deleted
    double &getF(int x,int y,int z) { return f.get(x,y,z);}
    double &getU(int x, int y,int z) { return u.get(x+1,y+1,z+1);}
    double &getU_old(int x, int y,int z) { return u_old.get(x+1,y+1,z+1);}
    double solve(int& iterations, double& tolerance,double hh);
};

double poissonSolver::solve(int& iterations, double& tolerance,double hh){ //returns error
    double error = 0.0;
     
    //These 2 exist since the error is defaulted to zero
    int num =0;
    int limit = 1;

    while(limit > num || tolerance<=error){ // ||
        #pragma omp parallel for collapse(3) reduction(+:error)
        for(int x=0;x<f.getWidth();x++){
            for(int y=0;y<f.getHeight();y++){
                for(int z=0;z<f.getDepth();z++){
                    getU(x,y,z) = (1.0/6.0)*(getU_old(x-1,y,z)+getU_old(x+1,y,z)+getU_old(x,y-1,z)+getU_old(x,y+1,z)+getU_old(x,y,z-1)+getU_old(x,y,z+1)+hh*getF(x,y,z));
                    error+=abs(getU(x,y,z)-getU_old(x,y,z));
                }
            }
        }
        
        error/=u.getWidth()*u.getHeight();

        u.swap(u_old);
        iterations++;
        num++;  
    }
   
    return error;
}



poissonSolver::~poissonSolver() //Works because grid uses destructors
{
}
