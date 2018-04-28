
#include <glm/vec3.hpp>
#include "../particle.h"
#include "../grid.h"
#include "plane.h"

#define SURFACE_OFFSET 0.0001
using namespace glm;

vec3 Plane::collide(vec3 position, vec3 next_position, vec3 velocity) {
    float test = dot(position - point, normal);
    float test_next = dot(next_position - point, normal);
    if (test * test_next < 0) { // the particle crosses over
        return vec3(0.0); // we want the particle to stop
    } else {
        return velocity;
    }
}

void Plane::render() {
  //TODO
//  nanogui::Color color(0.7f, 0.7f, 0.7f, 1.0f);
//
//  Vector3f sPoint(point.x, point.y, point.z);
//  Vector3f sNormal(normal.x, normal.y, normal.z);
//  Vector3f sParallel(normal.y - normal.z, normal.z - normal.x,
//                     normal.x - normal.y);
//  sParallel.normalize();
//  Vector3f sCross = sNormal.cross(sParallel);
//
//  MatrixXf positions(3, 4);
//  MatrixXf normals(3, 4);
//
//  positions.col(0) << sPoint + 2 * (sCross + sParallel);
//  positions.col(1) << sPoint + 2 * (sCross - sParallel);
//  positions.col(2) << sPoint + 2 * (-sCross + sParallel);
//  positions.col(3) << sPoint + 2 * (-sCross - sParallel);
//
//  normals.col(0) << sNormal;
//  normals.col(1) << sNormal;
//  normals.col(2) << sNormal;
//  normals.col(3) << sNormal;
//
//  if (shader.uniform("in_color", false) != -1) {
//    shader.setUniform("in_color", color);
//  }
//  shader.uploadAttrib("in_position", positions);
//  shader.uploadAttrib("in_normal", normals);
//
//  shader.drawArray(GL_TRIANGLE_STRIP, 0, 4);
}
