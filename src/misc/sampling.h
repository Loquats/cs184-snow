//
// Created by Brandon Huang on 4/28/18.
//

#ifndef SNOW_SAMPLING_H
#define SNOW_SAMPLING_H

#include "../grid.h"

void createOneToOneParticles(Grid *grid);

void createSphereUniformParticles(Grid *grid, int num_particles, float radius);

#endif //SNOW_SAMPLING_H
