#include "../grid.h"
#include "plane.h"

#define SURFACE_OFFSET 0.0001
using namespace glm;

vec3 Plane::collide(vec3 position, vec3 next_position, vec3 velocity) {
//    float test = dot(position - point, normal);
    float test_next = dot(next_position - point, normal);
    if (abs(test_next) < SURFACE_OFFSET) { // the particle crosses over
        return vec3(0.0); // we want the particle to stop
    } else {
        return velocity;
    }
}

void Plane::render(Shader *shader) {
  vec4 color(0.7f, 0.7f, 0.7f, 1.0f);

  shader->use();
  shader->setVec4("in_color", color);

  glm::mat4 model;
  model = glm::translate(model, glm::vec3(-float(dim.x)/2, -float(dim.y)/2, -float(dim.z)/2));
  shader->setMat4("model", model);
  glBindVertexArray(plane_VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
