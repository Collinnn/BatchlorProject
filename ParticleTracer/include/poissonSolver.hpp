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
    poissonSolver(domain_info domain) : f(domain.width/domain.cell_width,domain.height/domain.cell_height), u((domain.width/domain.cell_width)+2,(domain.height/domain.cell_height)+2), u_old((domain.width/domain.cell_width)+2,(domain.height/domain.cell_height)+2){};
    ~poissonSolver();
    poissonSolver(const poissonSolver &) = delete;            // Copy constructor deleted
    poissonSolver &operator=(const poissonSolver &) = delete; // Copy assignment operator deleted
    double &getF(int x,int y) { return f.get(x,y);}
    double &getU(int x, int y) { return u.get(x+1,y+1);}
    double &getU_old(int x, int y) { return u_old.get(x+1,y+1);}
    double solve(int& iterations, double tolerance);
};

double poissonSolver::solve(int& iterations, double tolerance){ //returns error
    double error = 0;

    //These 2 exist since the error is defaulted to zero
    int num =0;
    int limit = 100;

    while( limit > num){ //tolerance<=error ||
        #pragma omp parallel for collapse(2) reduction(+:error)
        for(int x=0;x<f.getWidth();x++){
            for(int y=0;y<f.getHeight();y++){
                getU(x,y) = 0.25*(getU_old(x-1,y)+getU_old(x+1,y)+getU_old(x,y-1)+getU_old(x,y+1)+getF(x,y));
                error+=abs(getU(x,y)-getU_old(x,y));

            }
        }
        
        error/=u.getWidth()*u.getHeight();
        u.swap(u_old);
        iterations++;
        num++;
    }
    return error;
}



poissonSolver::~poissonSolver()
{
}
