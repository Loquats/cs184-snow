#ifndef COLLISIONOBJECT_SPHERE_H
#define COLLISIONOBJECT_SPHERE_H

#include <glm/vec3.hpp>
#include "collisionObject.h"

using namespace glm;

struct Sphere : public CollisionObject {
public:
  Sphere(const vec3 &origin, double radius, double friction)
      : origin(origin), radius(radius), radius2(radius * radius),
        friction(friction) {}

  void render();
  void collide();

private:
  vec3 origin;
  double radius;
  double radius2;

  double friction;
};

#endif /* COLLISIONOBJECT_SPHERE_H */
