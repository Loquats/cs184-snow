

#include "../misc/sphere_drawing.h"
#include "sphere.h"

using namespace glm;

vec3 Sphere::collide(vec3 position, vec3 next_position, vec3 velocity) {
  //todo
    return vec3(0.0);
}

void Sphere::render() {
  // todo?
  // We decrease the radius here so flat triangles don't behave strangely
  // and intersect with the sphere when rendered
//  Misc::draw_sphere(shader, origin, radius * 0.92);
}
