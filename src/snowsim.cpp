// inspired by /copied from clothsim :)

#include <cmath>
#include <glad/glad.h>

#include "snowsim.h"

#include "camera.h"
#include "grid.h"
#include "collision/plane.h"
#include "collision/sphere.h"
#include "collision/rectangle.h"

using namespace std;

SnowSimulator::SnowSimulator(int frames_per_sec, int length, float delta_t, PhysicsParams* params) {
  this->frames_per_sec = frames_per_sec;
  this->length = length;
  this->delta_t = delta_t;
  this->params = params;

  this->external_accelerations = {gravity};
  float frame_duration = float(1. / frames_per_sec);
  this->simulation_steps = int(std::round(frame_duration / delta_t));
//  glEnable(GL_PROGRAM_POINT_SIZE);
//  glEnable(GL_DEPTH_TEST);
}

//SnowSimulator::~SnowSimulator() {
//  for (auto shader : shaders) {
//    shader.free();
//  }
//
//  if (cloth) delete cloth;
//  if (cp) delete cp;
//  if (collision_objects) delete collision_objects;
//}

void SnowSimulator::loadGrid(struct Grid *grid) {
  this->grid = grid;
}

void SnowSimulator::loadCollisionObjects(vector<CollisionObject *> *objects) {
  this->collision_objects = objects;
}

/**
 * Initializes the cloth simulation and spawns a new thread to separate
 * rendering from simulation.
 */
void SnowSimulator::init(Camera *camera, Shader *shader, glm::mat4 model) {
  this->camera = camera;
  this->shader = shader;
  this->modeltoworld = model;
  int x = grid->res_x;
  int y = grid->res_y;
  int z = grid->res_z;

  int num_lines = (z + 1) * ((x + 1) + (y + 1)) + ((x + 1) * (y+1));
  int num_vertices = num_lines * 2;
  float grid_vertices[num_vertices * 3];
  memset(grid_vertices, 0, num_vertices * 3 * sizeof(float));
  int t = 0;
  for (int z_coord = 0; z_coord <= z; ++z_coord) {
    for (int x_coord = 0; x_coord <= x; ++x_coord) {
      glm::vec3 bottom(float(x_coord), 0, float(z_coord));
      glm::vec3 top(float(x_coord), grid->res_y, float(z_coord));
      grid_vertices[t + 0] = bottom.x;
      grid_vertices[t + 1] = bottom.y;
      grid_vertices[t + 2] = bottom.z;
      grid_vertices[t + 3] = top.x;
      grid_vertices[t + 4] = top.y;
      grid_vertices[t + 5] = top.z;

      t += 6;
    }

    for (int y_coord = 0; y_coord <= y; ++y_coord) {
      // lines from x=0 to x=res_z at z=0
      glm::vec3 bottom(0.0f, float(y_coord), float(z_coord));
      glm::vec3 top(grid->res_x, float(y_coord), float(z_coord));
      grid_vertices[t + 0] = bottom.x;
      grid_vertices[t + 1] = bottom.y;
      grid_vertices[t + 2] = bottom.z;
      grid_vertices[t + 3] = top.x;
      grid_vertices[t + 4] = top.y;
      grid_vertices[t + 5] = top.z;

      t += 6;
    }
  }
  for (int x_coord = 0; x_coord <= x; ++x_coord) {
    for (int y_coord = 0; y_coord <= y; ++y_coord) {
      // lines from x=0 to x=res_z at z=0
      glm::vec3 front(float(x_coord), float(y_coord), 0.0);
      glm::vec3 back(float(x_coord), float(y_coord), grid->res_z);
      grid_vertices[t + 0] = front.x;
      grid_vertices[t + 1] = front.y;
      grid_vertices[t + 2] = front.z;
      grid_vertices[t + 3] = back.x;
      grid_vertices[t + 4] = back.y;
      grid_vertices[t + 5] = back.z;

      t += 6;
    }
  }

  glGenVertexArrays(1, &grid_VAO);
  glGenBuffers(1, &grid_VBO);

  glBindVertexArray(grid_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, grid_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  float vs = 0.5;
  GLfloat cube_vertices[] = {
    -vs,-vs,-vs, // triangle 1 : begin
    -vs,-vs, vs,
    -vs, vs, vs, // triangle 1 : end
    vs, vs,-vs, // triangle 2 : begin
    -vs,-vs,-vs,
    -vs, vs,-vs, // triangle 2 : end
    vs,-vs, vs,
    -vs,-vs,-vs,
    vs,-vs,-vs,
    vs, vs,-vs,
    vs,-vs,-vs,
    -vs,-vs,-vs,
    -vs,-vs,-vs,
    -vs, vs, vs,
    -vs, vs,-vs,
    vs,-vs, vs,
    -vs,-vs, vs,
    -vs,-vs,-vs,
    -vs, vs, vs,
    -vs,-vs, vs,
    vs,-vs, vs,
    vs, vs, vs,
    vs,-vs,-vs,
    vs, vs,-vs,
    vs,-vs,-vs,
    vs, vs, vs,
    vs,-vs, vs,
    vs, vs, vs,
    vs, vs,-vs,
    -vs, vs,-vs,
    vs, vs, vs,
    -vs, vs,-vs,
    -vs, vs, vs,
    vs, vs, vs,
    -vs, vs, vs,
    vs,-vs, vs
  };

  glGenVertexArrays(1, &particle_VAO);
  glGenBuffers(1, &particle_VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(particle_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, particle_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);



  glGenVertexArrays(1, &grid_force_VAO);
  glGenBuffers(1, &grid_force_VBO);

  glBindVertexArray(grid_force_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, grid_force_VBO);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);


  glGenVertexArrays(1, &particle_velocity_VAO);
  glGenBuffers(1, &particle_velocity_VBO);

  glBindVertexArray(particle_velocity_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, particle_velocity_VBO);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

}


void SnowSimulator::drawContents() {
  glEnable(GL_DEPTH_TEST);

  if (!is_paused) {
    clock_t start = clock();
    for (int i = 0; i < simulation_steps; i++) {
      // Not sure about the order of these updates:
      for (CollisionObject* co : *collision_objects) {
        if (!co->is_stationary()) {
          // TODO: make an abstract class called MovableCollisionObject for casting
          Rectangle* rect = (Rectangle*) co;
          rect->update_position(delta_t);
        }
      }
      grid->simulate(delta_t, external_accelerations, collision_objects, params);
    }
    double duration = (clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Frame time: " << duration << " seconds\n";
  }


  shader->use();
  // pass projection matrix to shader (note that in this case it could change every frame)
  glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  shader->setMat4("projection", projection);

  // camera/view transformation
  glm::mat4 view = camera->GetViewMatrix();
  shader->setMat4("view", view);
//  drawGrid(glm::vec4(0.8, 0.8, 0.8, 1.0));
  drawParticles(glm::vec4(0.7, 0.7, 0.7, 0.7));
//  drawGridForces();
//  drawParticleForces();
  for (CollisionObject *co : *collision_objects) {
    co->render(shader);
  }
}

void SnowSimulator::drawGrid(glm::vec4 color) {
  glBindVertexArray(grid_VAO);
  shader->setVec4("in_color", color);
  shader->setMat4("model", modeltoworld);
  int num_vertices = (grid->res_x + 1) * (grid->res_y + 1) * (grid->res_z + 1) * 2;

  glDrawArrays(GL_LINES, 0, num_vertices * 3);
}

void SnowSimulator::drawParticles(glm::vec4 color) {
  vector<Particle *>& all_particles = grid->all_particles;

  for(Particle* particle : all_particles) {
    float len = particle->cbrt_volume;
    glm::mat4 particle_model;
    particle_model = glm::scale(particle_model, glm::vec3(len, len, len));
    particle_model = modeltoworld * particle_model;
    particle_model = glm::translate(glm::mat4(), particle->position) * particle_model;
    shader->setVec4("in_color", color);
    shader->setMat4("model", particle_model);
    glBindVertexArray(particle_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
}

void SnowSimulator::drawGridForces() {

  float ARROW_LENGTH_SCALAR = 2;
  int num_arrows = grid->res_x * grid->res_y * grid->res_z;
  int num_vertices = num_arrows * 2;
  float grid_force_vertices[num_vertices * 3];
  memset(grid_force_vertices, 0, num_vertices * 3 * sizeof(float));

  int counter = 0;
  for (int i = 0; i < grid->res_x; ++i) {
    for (int j = 0; j < grid->res_y; ++j) {
      for (int k = 0; k < grid->res_z; ++k) {
        vec3* force = &grid->nodes[i][j][k]->force;
        if (force->x == 0 and force->y == 0 and force->z == 0) {
          continue;
        }
        vec3 center((i + 0.5) * grid->h, (j + 0.5) * grid->h, (k + 0.5) * grid->h);
        grid_force_vertices[counter] = center.x;
        grid_force_vertices[counter+1] = center.y;
        grid_force_vertices[counter+2] = center.z;
        counter += 3;
        vec3 copied_force(*force);
        copied_force *= ARROW_LENGTH_SCALAR;
        grid_force_vertices[counter] = center.x + copied_force.x;
        grid_force_vertices[counter+1] = center.y + copied_force.y;
        grid_force_vertices[counter+2] = center.z + copied_force.z;
        counter += 3;
      }
    }
  }
  shader->use();
  shader->setMat4("model", modeltoworld);
  glBindVertexArray(grid_force_VAO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(grid_force_vertices), grid_force_vertices, GL_DYNAMIC_DRAW);
  glDrawArrays(GL_LINES, 0, num_vertices);

}

void SnowSimulator::drawParticleForces() {

  float ARROW_LENGTH_SCALAR = 0.2;
  int num_arrows = int(grid->all_particles.size());
  int num_vertices = num_arrows * 2;
  float particle_velocity_vertices[num_vertices * 3];
  memset(particle_velocity_vertices, 0, num_vertices * 3 * sizeof(float));

  int counter = 0;
  for (Particle *p : grid->all_particles) {
    vec3* velocity = &p->velocity;
    if (velocity->x == 0 and velocity->y == 0 and velocity->z == 0) {
      continue;
    }
    vec3 center(p->position);
    particle_velocity_vertices[counter] = center.x;
    particle_velocity_vertices[counter+1] = center.y;
    particle_velocity_vertices[counter+2] = center.z;
    counter += 3;
    vec3 copied_velocity(*velocity);
    copied_velocity *= ARROW_LENGTH_SCALAR;
    particle_velocity_vertices[counter] = center.x + copied_velocity.x;
    particle_velocity_vertices[counter+1] = center.y + copied_velocity.y;
    particle_velocity_vertices[counter+2] = center.z + copied_velocity.z;
    counter += 3;
  }
  shader->use();
  shader->setMat4("model", modeltoworld);
  glBindVertexArray(particle_velocity_VAO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_velocity_vertices), particle_velocity_vertices, GL_DYNAMIC_DRAW);
  glDrawArrays(GL_LINES, 0, num_vertices);

}
