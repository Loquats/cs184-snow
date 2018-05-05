#include "cube.h"
#include "rectangle.h"

using namespace glm;

vec3 Cube::collide(vec3 position, vec3 velocity, float delta_t) {
  // TODO: this seems kinda change
  for (Rectangle* rect : faces) {
    velocity = rect->collide(position, velocity, delta_t);
  }
  return velocity;
}

void Cube::render(Shader *shader) {
  for (Rectangle* rect : faces) {
    rect->render(shader);
  }
}

bool Cube::is_stationary() {
  // TODO: stationary if velocity magnitude is less than epsilon
  return false;
}

void Cube::set_velocity(vec3 velocity) {
  for (Rectangle* rect : faces) {
    rect->set_velocity(velocity);
  }
}

void Cube::update_position(float delta_t) {
  for (Rectangle* rect : faces) {
    rect->update_position(delta_t);
  }
}
