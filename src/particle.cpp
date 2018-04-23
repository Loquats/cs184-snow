#include <iostream>
#include <glm/ext.hpp>
#include "particle.h"

void test_particle() {
  float mass = 5;
  glm::vec3 cool_vec = glm::vec3(1.0, 2.0, 3.0);
  Particle* particle = new Particle(cool_vec, mass);
  printf("particle position: %f %f %f\n", particle->position.x, particle->position.y, particle->position.z);
  printf("particle velocity: %f %f %f\n", particle->velocity.x, particle->velocity.y, particle->velocity.z);
  std::cout << glm::to_string(particle->deformation_grad) << "\n"; // <- to_string... nice!!

  std::cout << glm::to_string(particle->position * 0.5f) << "\n"; // <- to_string... nice!!
}