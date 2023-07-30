#pragma once
#include "./particles.hpp"
#include "./util.hpp"

int toPPMfile(domain_info& domain, particle_storage &storage, int& iter);
void toCSVfile(timeZone& time);
int printArr(domain_info& domain, particle_storage &storage);