
#include "plane.h"

#define SURFACE_OFFSET 0.0001

void Plane::collide() {
  // TODO
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
