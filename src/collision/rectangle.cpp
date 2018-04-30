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
      // Friction collision
      vec3 outward_normal;
      if (dot(position - origin, normal) > 0) {
        outward_normal = normal;
      } else {
        outward_normal = -normal;
      }
      float v_n = dot(velocity, outward_normal);
      vec3 velocity_tangent = velocity - outward_normal * v_n;
      float mag_velocity_tangent = length(velocity_tangent);
      if (mag_velocity_tangent <= -mu * v_n) {
        // Static friction
        return vec3(0.0);
      } else {
        // Dynamic friction
        return (1 + mu * v_n / mag_velocity_tangent) * velocity_tangent;
      }
    }
  }
  // No collision
  return velocity;
}

void Rectangle::render(Shader *shader) {
  shader->use();
  shader->setVec4("in_color", color);
  shader->setMat4("model", model);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, num_vertices);
}
