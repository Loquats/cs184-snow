//
// Created by Brandon Huang on 4/28/18.
//

#include "sampling.h"
#include <glm/gtc/random.hpp>

using namespace glm;


void createSphereUniformParticles(Grid *grid, int num_particles, float radius) {
    vec3 grid_center = vec3(grid->dim_x, grid->dim_y, grid->dim_z)/2.0f;
    grid_center.y += grid->dim_y * 0.2;
    for (int i = 0; i < num_particles; i++) {
        Particle* p = new Particle(grid_center + ballRand(radius), 10, vec3(grid->dim_x, grid->dim_y, grid->dim_z), grid->h);
        p->velocity = vec3(0.0);
        grid->all_particles.push_back(p);
    }
    grid->resetGrid();
}
