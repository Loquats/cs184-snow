#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

struct Particle {

  Particle(glm::vec3 position, float mass): 
    position(position), mass(mass) {};

  glm::vec3 position;
  glm::vec3 velocity;
  float mass;
  float volume;

  glm::mat3 deformation_grad_E;
  glm::mat3 deformation_grad_P;

  // for step 3
  glm::mat3 F_hat_Ep;
};


///////////////////////////////////////////////////////////////////

#endif /* PARTICLE_H */