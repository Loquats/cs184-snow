//
// Created by Brandon Huang on 4/28/18.
//

#include "sampling.h"
#include <glm/gtc/random.hpp>

using namespace glm;

void createOneToOneParticles(Grid *grid) {
    for (int i = 0; i < grid->dim_x; ++i) {
        for (int j = 0; j < grid->dim_y; ++j) {
            for (int k = 0; k < grid->dim_z; ++k) {
                Particle* a_particle = new Particle(vec3(float(i) + 0.5, float(j) + 0.5, float(k) + 0.5), 10);
                a_particle->velocity = vec3(0.0);
                grid->all_particles.push_back(a_particle);
            }
        }
    }
    grid->resetGrid();
}

void createSphereUniformParticles(Grid *grid, int num_particles, float radius) {
    vec3 grid_center = vec3(grid->dim_x, grid->dim_y, grid->dim_z)/2.0f;
    grid_center.y += grid->dim_y * 0.15;
    for (int i = 0; i < num_particles; i++) {
        Particle* p = new Particle(grid_center + ballRand(radius), 10);
        p->velocity = vec3(0.0);
        grid->all_particles.push_back(p);
    }
    grid->resetGrid();
}

void createTwoParticles(Grid *grid) {
    vec3 center = vec3(grid->dim_x, grid->dim_y, grid->dim_z)/2.0f;
    Particle* p = new Particle(center + vec3(5, 0, 0), 1);
    p->velocity = vec3(0.0);
    grid->all_particles.push_back(p);

    Particle* q = new Particle(center + vec3(-5, 0, 0), 1);
    q->velocity = vec3(0.0);
    grid->all_particles.push_back(q);

    Particle* origin = new Particle(vec3(0.1, 0.1, 0.1), 1);
    origin->velocity = vec3(0.0);
    grid->all_particles.push_back(origin);    

    grid->resetGrid();
  }