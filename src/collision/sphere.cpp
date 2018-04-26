

#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace glm;

void Sphere::collide() {
  //todo
}

void Sphere::render() {
  // todo?
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
//  Misc::draw_sphere(shader, origin, radius * 0.92);
}
