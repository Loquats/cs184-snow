#ifndef COLLISIONOBJECT_RECTANGLE_H
#define COLLISIONOBJECT_RECTANGLE_H

#include <glad/glad.h>
#include "collisionObject.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct Rectangle : public CollisionObject {
public:
  // input is in model space (0, 0, 0) to (dimx, dimy, dimz)
  Rectangle(vec3 &in_origin, vec3 &edge_u, vec3 &edge_v, float mu, mat4 model, mat4 worldtomodel, vec4 color)
    : edge_u(edge_u), edge_v(edge_v), mu(mu), modeltoworld(model), worldtomodel(worldtomodel), color(color) {
      // Check if rectangle is valid: edges must be orthogonal
      if (abs(dot(edge_u, edge_v)) > 0) {
        throw "bad rectangle";
      }

      origin = vec3(worldtomodel * vec4(in_origin, 1.0));
      normal = normalize(cross(edge_u, edge_v));

      vec3 point1 = origin + edge_u;
      vec3 point2 = origin + edge_u + edge_v;
      vec3 point3 = origin + edge_v;

      vertices[0] = origin.x;
      vertices[1] = origin.y;
      vertices[2] = origin.z;
      vertices[3] = point1.x;
      vertices[4] = point1.y;
      vertices[5] = point1.z;
      vertices[6] = point2.x;
      vertices[7] = point2.y;
      vertices[8] = point2.z;
      vertices[9] = point3.x;
      vertices[10] = point3.y;
      vertices[11] = point3.z;

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
  }

  void render(Shader *shader);
  vec3 collide(vec3 position, vec3 next_position, vec3 velocity);

private:
  // For collision
  vec3 origin;
  vec3 edge_u;
  vec3 edge_v;
  vec3 normal;
  float mu;     // friction coefficient

  // For rendering
  const int static num_vertices = 4;
  float vertices[num_vertices * 3];
  unsigned int VAO, VBO;
  mat4 modeltoworld;
  mat4 worldtomodel;
  vec4 color;
};

#endif /* COLLISIONOBJECT_RECTANGLE_H */
