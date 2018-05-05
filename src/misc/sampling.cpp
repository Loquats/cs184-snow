//
// Created by Brandon Huang on 4/28/18.
//

#include "sampling.h"
#include <glm/gtc/random.hpp>

using namespace glm;


void createSphereUniformParticles(Grid *grid, int num_particles, float radius) {
    vec3 grid_center = vec3(grid->dim_x, grid->dim_y, grid->dim_z)/2.0f;
    grid_center.y += grid->dim_y * 0.15;
    for (int i = 0; i < num_particles; i++) {
        Particle* p = new Particle(grid_center + ballRand(radius), 10, vec3(grid->res_x, grid->res_y, grid->res_z), grid->h);
        p->velocity = vec3(0.0);
        grid->all_particles.push_back(p);
    }
    grid->resetGrid();
}

void createTower(Grid *grid, int num_particles) {
    vec3 grid_dim = vec3(grid->dim_x, grid->dim_y, grid->dim_z);
    vec3 tower_dim = vec3(grid_dim.x * 0.7, grid_dim.y * 0.1, grid_dim.z * 0.7);
    vec3 tower_origin = vec3(grid_dim.x - tower_dim.x, 0.0, grid_dim.z - tower_dim.z) / 2.0f;

    for (int i = 0; i < num_particles; i++) {
        vec3 sample_position = linearRand(tower_origin, tower_origin + tower_dim);
        Particle* p = new Particle(sample_position, 10, vec3(grid->res_x, grid->res_y, grid->res_z), grid->h);
        p->velocity = vec3(0.0);
        grid->all_particles.push_back(p);
    }
    grid->resetGrid();
}