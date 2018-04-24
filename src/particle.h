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

  // I think deformation gradient is a 3x3 matrix but not totally sure
  glm::mat3 deformation_grad_E;
  glm::mat3 deformation_grad_P;
  // ^ these are by default an identity matrix

  // for step 3
  glm::mat3 F_hat_Ep;
};


///////////////////////////////////////////////////////////////////

#endif /* PARTICLE_H */