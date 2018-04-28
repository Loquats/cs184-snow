#ifndef COLLISIONOBJECT_PLANE_H
#define COLLISIONOBJECT_PLANE_H


#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include "../particle.h"
#include "collisionObject.h"

using namespace glm;

struct Plane : public CollisionObject {
public:
  Plane(const vec3 &point, const vec3 &normal, double friction)
      : point(point), normal(normal / l2Norm(normal)), friction(friction) {}

  void render();
  vec3 collide(vec3 position, vec3 next_position, vec3 velocity);

  vec3 point;
  vec3 normal;

  double friction;
private:

};

#endif /* COLLISIONOBJECT_PLANE_H */
