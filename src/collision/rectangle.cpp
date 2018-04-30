#include <glm/gtx/norm.hpp>
#include "../grid.h"
#include "rectangle.h"

#define SURFACE_OFFSET 0.0001
using namespace glm;

vec3 Rectangle::collide(vec3 position, vec3 next_position, vec3 velocity) {
  // Check if next_position enters infinite plane
  vec3 next_position_origin = next_position - origin;
  float offset = dot(next_position_origin, normal);
  if (abs(offset) < SURFACE_OFFSET) {
    // Check if next_position is within finite bounds

    // Projection to plane
    vec3 next_position_plane = next_position_origin - normal * dot(next_position_origin, normal);
    float proj_u = dot(next_position_plane, edge_u);
    float proj_v = dot(next_position_plane, edge_v);
    if (proj_u > 0 && proj_u < length2(edge_u)
     && proj_v > 0 && proj_v < length2(edge_v)) {
      // Simple collision: stop
      return vec3(0.0);
    }
  }
  return velocity;
}

void Rectangle::render(Shader *shader) {
  shader->use();
  shader->setVec4("in_color", color);
  shader->setMat4("model", model);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, num_vertices);
}
