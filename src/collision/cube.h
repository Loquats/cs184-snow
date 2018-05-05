#ifndef COLLISIONOBJECT_CUBE_H
#define COLLISIONOBJECT_CUBE_H

#include <glad/glad.h>
#include "collisionObject.h"
#include "rectangle.h"

using namespace glm;

// technically could be a rectangular prism
struct Cube : public CollisionObject {
public:
  Cube(vec3 &origin_world, vec3 &edge_u, vec3 &edge_v, vec3 &edge_w, float mu, mat4 model, mat4 worldtomodel, vec4 color) {
    // edges must be orthogonal
    if (abs(dot(edge_u, edge_v)) > 0 || abs(dot(edge_u, edge_w)) > 0 || abs(dot(edge_v, edge_w)) > 0) {
      throw "bad cube";
    }

    faces[0] = new Rectangle(origin_world, edge_u, edge_v, mu, model, worldtomodel, color);
    faces[1] = new Rectangle(origin_world, edge_u, edge_w, mu, model, worldtomodel, color);
    faces[2] = new Rectangle(origin_world, edge_v, edge_w, mu, model, worldtomodel, color);
    vec3 origin_u = origin_world + edge_u;
    vec3 origin_v = origin_world + edge_v;
    vec3 origin_w = origin_world + edge_w;
    faces[3] = new Rectangle(origin_u, edge_v, edge_w, mu, model, worldtomodel, color);
    faces[4] = new Rectangle(origin_v, edge_u, edge_w, mu, model, worldtomodel, color);
    faces[5] = new Rectangle(origin_w, edge_u, edge_v, mu, model, worldtomodel, color);
  }

  void render(Shader *shader);
  vec3 collide(vec3 position, vec3 velocity, float delta_t);
  bool is_stationary();
  void set_velocity(vec3 velocity);
  void update_position(float delta_t);

private:
  Rectangle* faces[6];
};

#endif /* COLLISIONOBJECT_CUBE_H */
