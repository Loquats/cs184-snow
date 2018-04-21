#include <glm/glm.hpp>

struct Particle {

  Particle(glm::vec3 position, float mass): 
    position(position), mass(mass) {};

  glm::vec3 position;
  glm::vec3 velocity;
  float mass;
  float volume;

  // I think deformation gradient is a 3x3 matrix but not totally sure
  glm::mat3 deformation_grad;
  // ^ this is by default an identity matrix
};


///////////////////////////////////////////////////////////////////

void test_particle() {
    float mass = 5;
    glm::vec3 cool_vec = glm::vec3(1.0, 2.0, 3.0);
    Particle* particle = new Particle(cool_vec, mass);
    printf("particle position: %f %f %f\n", particle->position.x, particle->position.y, particle->position.z);
    printf("particle velocity: %f %f %f\n", particle->velocity.x, particle->velocity.y, particle->velocity.z);
    std::cout << glm::to_string(particle->deformation_grad) << "\n"; // <- to_string... nice!!

    std::cout << glm::to_string(particle->position * 0.5f) << "\n"; // <- to_string... nice!!
}