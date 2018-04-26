#ifndef COLLISIONOBJECT_PLANE_H
#define COLLISIONOBJECT_PLANE_H


#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include "collisionObject.h"

using namespace glm;

struct Plane : public CollisionObject {
public:
  Plane(const vec3 &point, const vec3 &normal, double friction)
      : point(point), normal(normal / l2Norm(normal)), friction(friction) {}

  void render();
  void collide();

  vec3 point;
  vec3 normal;

  double friction;
};

#endif /* COLLISIONOBJECT_PLANE_H */
