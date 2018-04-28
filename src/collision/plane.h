#ifndef COLLISIONOBJECT_PLANE_H
#define COLLISIONOBJECT_PLANE_H


#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "collisionObject.h"
#include "../shader.h"

using namespace glm;

struct Plane : public CollisionObject {
public:
  Plane(const vec3 &point, const vec3 &normal)
      : point(point), normal(normal / l2Norm(normal)) {
    vec3 sPoint(point.x, point.y, point.z);
    vec3 sNormal(normal.x, normal.y, normal.z);
    vec3 sParallel(normal.y - normal.z, normal.z - normal.x,
                   normal.x - normal.y);
    sParallel = normalize(sParallel);
    vec3 sCross = cross(sNormal, sParallel);


    memset(plane_vertices, 0, num_vertices * 3 * sizeof(float));
    vec3 p1 = sPoint + 2.0f * sCross;
    vec3 p2 = sPoint + 2.0f * -sParallel;
    vec3 p3 = sPoint + 2.0f * sParallel;
    vec3 p4 = sPoint + 2.0f * -sCross;
//    vec3 p1 = sPoint + 2.0f * (sCross + sParallel);
//    vec3 p3 = sPoint + 2.0f * (sCross - sParallel);
//    vec3 p2 = sPoint + 2.0f * (-sCross + sParallel);
//    vec3 p4 = sPoint + 2.0f * (-sCross - sParallel);

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

//    plane_vertices[0] = *glm::value_ptr(p1);
//    plane_vertices[3] = *glm::value_ptr(p2);
//    plane_vertices[6] = *glm::value_ptr(p3);
//    plane_vertices[9] = *glm::value_ptr(p4);

    glGenVertexArrays(1, &plane_VAO);
    glGenBuffers(1, &plane_VBO);

    glBindVertexArray(plane_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

  }

  void render(Shader *shader);
  void collide();

  const int static num_vertices = 4;
  float plane_vertices[num_vertices * 3];
  vec3 point;
  vec3 normal;

  unsigned int plane_VAO, plane_VBO;

};

#endif /* COLLISIONOBJECT_PLANE_H */
