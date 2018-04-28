
#include "plane.h"
#include <Eigen/Dense>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#define SURFACE_OFFSET 0.0001

using namespace Eigen;

void Plane::collide() {
  // TODO
}

void Plane::render(Shader *shader) {
  //TODO
  vec4 color(0.7f, 0.7f, 0.7f, 1.0f);
  glBindVertexArray(plane_VAO);
  shader->use();
  shader->setVec4("in_color", color);


  glm::mat4 model(1.0f);
  std::cout<<glm::to_string(model)<<std::endl;
  shader->setMat4("model", model);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
