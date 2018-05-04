#ifndef COLLISIONOBJECT_PLANE_H
#define COLLISIONOBJECT_PLANE_H


#include <glm/gtx/norm.hpp>
#include "../particle.h"
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "collisionObject.h"

using namespace glm;

struct Plane : public CollisionObject {
public:
  Plane(vec3 &point_in, vec3 &normal_in, const vec3 dim, float mu, glm::mat4 worldtomodel, glm::mat4 modeltoworld)
      : dim(dim), mu(mu), worldtomodel(worldtomodel), modeltoworld(modeltoworld) {

    point = vec3(worldtomodel * vec4(point_in, 1.0));
    normal = vec3(worldtomodel * vec4(normal_in, 0.0));
    vec3 sParallel(normal.y - normal.z, normal.z - normal.x,
                   normal.x - normal.y);
    sParallel = normalize(sParallel);
    vec3 sCross = cross(normal, sParallel);

    memset(plane_vertices, 0, num_vertices * 3 * sizeof(float));
    vec3 p1 = point + 200.0f * sCross;
    vec3 p2 = point + 200.0f * -sParallel;
    vec3 p3 = point + 200.0f * sParallel;
    vec3 p4 = point + 200.0f * -sCross;

    plane_vertices[0] = p1.x;
    plane_vertices[1] = p1.y;
    plane_vertices[2] = p1.z;
    plane_vertices[3] = p2.x;
    plane_vertices[4] = p2.y;
    plane_vertices[5] = p2.z;
    plane_vertices[6] = p3.x;
    plane_vertices[7] = p3.y;
    plane_vertices[8] = p3.z;
    plane_vertices[9] = p4.x;
    plane_vertices[10] = p4.y;
    plane_vertices[11] = p4.z;

    glGenVertexArrays(1, &plane_VAO);
    glGenBuffers(1, &plane_VBO);

    glBindVertexArray(plane_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

  }

  void render(Shader *shader);
  vec3 collide(vec3 position, vec3 next_position, vec3 velocity);
  bool is_stationary();

  const int static num_vertices = 4;
  float plane_vertices[num_vertices * 3];
  vec3 point;
  vec3 normal;
  vec3 dim;

  mat4 worldtomodel;
  mat4 modeltoworld;
private:
  unsigned int plane_VAO, plane_VBO;
  float mu;
};

#endif /* COLLISIONOBJECT_PLANE_H */
