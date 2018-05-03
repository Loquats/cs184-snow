#include "../grid.h"
#include "plane.h"

#define SURFACE_OFFSET 0.0001
using namespace glm;

vec3 Plane::collide(vec3 position, vec3 next_position, vec3 velocity) {
    float test = dot(position - point, normal);
    float test_next = dot(next_position - point, normal);
    if (abs(test_next) < SURFACE_OFFSET || test * test_next < 0) { // the particle crosses over
      // Friction collision
      vec3 outward_normal;
      if (dot(position - point, normal) > 0) {
        outward_normal = normal;
      } else {
        outward_normal = -normal;
      }
      outward_normal = normalize(outward_normal);
      float v_n = dot(velocity, outward_normal);
      vec3 velocity_tangent = velocity - outward_normal * v_n;
      float mag_velocity_tangent = length(velocity_tangent);
      if (mag_velocity_tangent <= -mu * v_n) {
        // Static friction
        return vec3(0.0);
      } else {
        // Dynamic friction
        return (1 + mu * v_n / mag_velocity_tangent) * velocity_tangent;
      }    } else {
        return velocity;
    }
}

void Plane::render(Shader *shader) {
  vec4 color(0.7f, 0.7f, 0.7f, 1.0f);

  shader->use();
  shader->setVec4("in_color", color);

  shader->setMat4("model", modeltoworld);
  glBindVertexArray(plane_VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
