#include "./include/particles.hpp"
#include "./include/particleTypes.hpp"
#pragma once

void get_density(const particle_storage &particles, grid<double> &density){
    size_t stride = density.getWidth();
    
    //resets densities to zero
    for(int y=0; y<density.getHeight(); y++){
        for(int x=0; x<density.getWidth(); x++){
            density[y*stride + x] = 0;
        }
    }
    const domain_info &domain = {};

    for(int i=0; i<particles.getSize(); i++){
        const particle2D &p = particles[i];
        int x = (p.x - domain.physical_width/2)/domain.cell_width;
        int y = (p.y - domain.physical_height/2)/domain.cell_height;
        density[y*stride + x] += p.weight;
    }

}