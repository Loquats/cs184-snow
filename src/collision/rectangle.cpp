#include <glm/gtx/norm.hpp>
#include "../grid.h"
#include "rectangle.h"

#define SURFACE_OFFSET 0.0001
using namespace glm;

vec3 Rectangle::collide(vec3 position, vec3 velocity, float delta_t) {
  vec3 velocity_rel = velocity - this->object_velocity;
  vec3 origin_model = vec3(worldtomodel * vec4(this->origin, 1.0));
  vec3 next_origin_model = vec3(worldtomodel * vec4(this->origin + object_velocity * delta_t, 1.0));
  vec3 next_position = position + velocity * delta_t;

  // Check if next_position enters infinite plane
  vec3 next_position_origin = next_position - origin_model;
  float offset = dot(position - origin_model, normal);
  float offset_next = dot(next_position - next_origin_model, normal);
  if (abs(offset) < SURFACE_OFFSET ||  offset * offset_next < 0) {
    // Check if next_position is within finite bounds

    // Projection to plane
    vec3 next_position_plane = next_position_origin - normal * dot(next_position_origin, normal);
    float proj_u = dot(next_position_plane, edge_u);
    float proj_v = dot(next_position_plane, edge_v);
    if (proj_u > 0 && proj_u < length2(edge_u)
     && proj_v > 0 && proj_v < length2(edge_v)) {
      // Friction collision
      vec3 outward_normal;
      if (dot(position - origin_model, normal) > 0) {
        outward_normal = normal;
      } else {
        outward_normal = -normal;
      }
      float v_n = dot(velocity_rel, outward_normal);
      vec3 velocity_tangent = velocity_rel - outward_normal * v_n;
      float mag_velocity_tangent = length(velocity_tangent);
      if (mag_velocity_tangent <= -mu * v_n) {
        // Static friction
        return object_velocity + vec3(0.0);
      } else {
        // Dynamic friction
        return object_velocity + ((1 + mu * v_n / mag_velocity_tangent) * velocity_tangent);
      }
    }
  }
  // No collision
  return velocity;
}

void Rectangle::render(Shader *shader) {
  shader->use();
  shader->setVec4("in_color", color);
  glm::mat4 model = translate(mat4(), origin);
  shader->setMat4("model", model);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_FAN, 0, num_vertices);
}

bool Rectangle::is_stationary() {
  // TODO: stationary if velocity magnitude is less than epsilon
  return false;
}

void Rectangle::set_velocity(vec3 velocity) {
  this->object_velocity = velocity;
}

void Rectangle::update_position(float delta_t) {
  // Explicit update for origin
  origin += object_velocity * delta_t;
}
