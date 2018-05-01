// inspired by /copied from clothsim :)

#include <cmath>
#include <glad/glad.h>

#include "snowsim.h"

#include "camera.h"
#include "grid.h"
#include "collision/plane.h"
#include "collision/sphere.h"

using namespace std;

SnowSimulator::SnowSimulator(int frames_per_sec, int length) {

  this->frames_per_sec = frames_per_sec;
  this->length = length;
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);
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

//void SnowSimulator::loadClothParameters(ClothParameters *cp) { this->cp = cp; }

void SnowSimulator::loadCollisionObjects(vector<CollisionObject *> *objects) { this->collision_objects = objects; }

/**
 * Initializes the cloth simulation and spawns a new thread to separate
 * rendering from simulation.
 */
void SnowSimulator::init(Camera *camera, Shader *shader, glm::mat4 model) {
  this->camera = camera;
  this->shader = shader;
  this->model = model;
  int x = grid->dim_x;
  int y = grid->dim_y;
  int z = grid->dim_z;

  int num_lines = (z + 1) * ((x + 1) + (y + 1)) + ((x + 1) * (y+1));
  int num_vertices = num_lines * 2;
  float grid_vertices[num_vertices * 3];
  memset(grid_vertices, 0, num_vertices * 3 * sizeof(float));
  int t = 0;
  for (int z_coord = 0; z_coord <= z; ++z_coord) {
    for (int x_coord = 0; x_coord <= x; ++x_coord) {
      glm::vec3 bottom(float(x_coord), 0, float(z_coord));
      glm::vec3 top(float(x_coord), grid->dim_y, float(z_coord));
      grid_vertices[t + 0] = bottom.x;
      grid_vertices[t + 1] = bottom.y;
      grid_vertices[t + 2] = bottom.z;
      grid_vertices[t + 3] = top.x;
      grid_vertices[t + 4] = top.y;
      grid_vertices[t + 5] = top.z;

      t += 6;
    }

    for (int y_coord = 0; y_coord <= y; ++y_coord) {
      // lines from x=0 to x=dim_z at z=0
      glm::vec3 bottom(0.0f, float(y_coord), float(z_coord));
      glm::vec3 top(grid->dim_x, float(y_coord), float(z_coord));
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
      // lines from x=0 to x=dim_z at z=0
      glm::vec3 front(float(x_coord), float(y_coord), 0.0);
      glm::vec3 back(float(x_coord), float(y_coord), grid->dim_z);
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

  float vs = 0.2;
  GLfloat cube_vertices[] = {
          -vs, -vs, -vs,   -vs, -vs,  vs,   -vs,  vs,  vs,   -vs,  vs, -vs,
          vs, -vs, -vs,    vs, -vs,  vs,    vs,  vs,  vs,    vs,  vs, -vs,
          -vs, -vs, -vs,   -vs, -vs,  vs,    vs, -vs,  vs,    vs, -vs, -vs,
          -vs,  vs, -vs,   -vs,  vs,  vs,    vs,  vs,  vs,    vs,  vs, -vs,
          -vs, -vs, -vs,   -vs,  vs, -vs,    vs,  vs, -vs,    vs, -vs, -vs,
          -vs, -vs,  vs,   -vs,  vs,  vs,    vs,  vs,  vs,    vs, -vs,  vs
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
  // TODO
  glEnable(GL_DEPTH_TEST);

  if (!is_paused) {
    vector<vec3> external_accelerations = {gravity};
    float frame_time = float(1. / frames_per_sec);
    int simulation_steps = int(std::round(frame_time / delta_t));
    clock_t start = clock();
    float E0 = 1.4e5;
    for (int i = 0; i < simulation_steps; i++) {
      grid->simulate(delta_t, external_accelerations, collision_objects, E0);
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
  // drawGrid(glm::vec4(0.8, 0.8, 0.8, 1.0));
  drawParticles(glm::vec4(0.7, 0.7, 0.7, 1.0));
//  drawGridForces();
  drawParticleForces();
  for (CollisionObject *co : *collision_objects) {
    co->render(shader);
  }
}

void SnowSimulator::drawGrid(glm::vec4 color) {
  glBindVertexArray(grid_VAO);
  shader->setVec4("in_color", color);
  shader->setMat4("model", model);
  int num_vertices = (grid->dim_x + 1) * (grid->dim_y + 1) * (grid->dim_z + 1) * 2;

  glDrawArrays(GL_LINES, 0, num_vertices * 3);
}

void SnowSimulator::drawParticles(glm::vec4 color) {
  vector<Particle *>& all_particles = grid->all_particles;

  for(Particle* particle : all_particles) {
    glm::mat4 particle_model = glm::translate(model, particle->position);
    shader->setVec4("in_color", color);
    shader->setMat4("model", particle_model);
    glBindVertexArray(particle_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 24);
  }


}

void SnowSimulator::drawGridForces() {

  float ARROW_LENGTH_SCALAR = 2;
  int num_arrows = grid->dim_x * grid->dim_y * grid->dim_z;
  int num_vertices = num_arrows * 2;
  float grid_force_vertices[num_vertices * 3];
  memset(grid_force_vertices, 0, num_vertices * 3 * sizeof(float));

  int counter = 0;
  for (int i = 0; i < grid->dim_x; ++i) {
    for (int j = 0; j < grid->dim_y; ++j) {
      for (int k = 0; k < grid->dim_z; ++k) {
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
  shader->setMat4("model", model);
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
  shader->setMat4("model", model);
  glBindVertexArray(particle_velocity_VAO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_velocity_vertices), particle_velocity_vertices, GL_DYNAMIC_DRAW);
  glDrawArrays(GL_LINES, 0, num_vertices);

}

// ----------------------------------------------------------------------------
// CAMERA CALCULATIONS
//
// OpenGL 3.1 deprecated the fixed pipeline, so we lose a lot of useful OpenGL
// functions that have to be recreated here.
// ----------------------------------------------------------------------------
// TODO CAMERA SHIT
//void SnowSimulator::resetCamera() { camera.copy_placement(canonicalCamera); }
//
//Matrix4f SnowSimulator::getProjectionMatrix() {
//  Matrix4f perspective;
//  perspective.setZero();
//
//  double near = camera.near_clip();
//  double far = camera.far_clip();
//
//  double theta = camera.v_fov() * M_PI / 360;
//  double range = far - near;
//  double invtan = 1. / tanf(theta);
//
//  perspective(0, 0) = invtan / camera.aspect_ratio();
//  perspective(1, 1) = invtan;
//  perspective(2, 2) = -(near + far) / range;
//  perspective(3, 2) = -1;
//  perspective(2, 3) = -2 * near * far / range;
//  perspective(3, 3) = 0;
//
//  return perspective;
//}
//
//Matrix4f SnowSimulator::getViewMatrix() {
//  Matrix4f lookAt;
//  Matrix3f R;
//
//  lookAt.setZero();
//
//  // Convert CGL vectors to Eigen vectors
//  // TODO: Find a better way to do this!
//
//  CGL::Vector3D c_pos = camera.position();
//  CGL::Vector3D c_udir = camera.up_dir();
//  CGL::Vector3D c_target = camera.view_point();
//
//  Vector3f eye(c_pos.x, c_pos.y, c_pos.z);
//  Vector3f up(c_udir.x, c_udir.y, c_udir.z);
//  Vector3f target(c_target.x, c_target.y, c_target.z);
//
//  R.col(2) = (eye - target).normalized();
//  R.col(0) = up.cross(R.col(2)).normalized();
//  R.col(1) = R.col(2).cross(R.col(0));
//
//  lookAt.topLeftCorner<3, 3>() = R.transpose();
//  lookAt.topRightCorner<3, 1>() = -R.transpose() * eye;
//  lookAt(3, 3) = 1.0f;
//
//  return lookAt;
//}
//
//// ----------------------------------------------------------------------------
//// EVENT HANDLING
//// ----------------------------------------------------------------------------
//
//bool SnowSimulator::cursorPosCallbackEvent(double x, double y) {
//  if (left_down && !middle_down && !right_down) {
//    if (ctrl_down) {
//      mouseRightDragged(x, y);
//    } else {
//      mouseLeftDragged(x, y);
//    }
//  } else if (!left_down && !middle_down && right_down) {
//    mouseRightDragged(x, y);
//  } else if (!left_down && !middle_down && !right_down) {
//    mouseMoved(x, y);
//  }
//
//  mouse_x = x;
//  mouse_y = y;
//
//  return true;
//}
//
//bool SnowSimulator::mouseButtonCallbackEvent(int button, int action,
//                                              int modifiers) {
//  switch (action) {
//    case GLFW_PRESS:
//      switch (button) {
//        case GLFW_MOUSE_BUTTON_LEFT:
//          left_down = true;
//          break;
//        case GLFW_MOUSE_BUTTON_MIDDLE:
//          middle_down = true;
//          break;
//        case GLFW_MOUSE_BUTTON_RIGHT:
//          right_down = true;
//          break;
//      }
//      return true;
//
//    case GLFW_RELEASE:
//      switch (button) {
//        case GLFW_MOUSE_BUTTON_LEFT:
//          left_down = false;
//          break;
//        case GLFW_MOUSE_BUTTON_MIDDLE:
//          middle_down = false;
//          break;
//        case GLFW_MOUSE_BUTTON_RIGHT:
//          right_down = false;
//          break;
//      }
//      return true;
//  }
//
//  return false;
//}
//
//void SnowSimulator::mouseMoved(double x, double y) { y = screen_h - y; }
//
//void SnowSimulator::mouseLeftDragged(double x, double y) {
//  float dx = x - mouse_x;
//  float dy = y - mouse_y;
//
//  camera.rotate_by(-dy * (PI / screen_h), -dx * (PI / screen_w));
//}
//
//void SnowSimulator::mouseRightDragged(double x, double y) {
//  camera.move_by(mouse_x - x, y - mouse_y, canonical_view_distance);
//}
//
//bool SnowSimulator::keyCallbackEvent(int key, int scancode, int action,
//                                      int mods) {
//  ctrl_down = (bool)(mods & GLFW_MOD_CONTROL);
//
//  if (action == GLFW_PRESS) {
//    switch (key) {
//      case GLFW_KEY_ESCAPE:
//        is_alive = false;
//        break;
//      case 'r':
//      case 'R':
//        cloth->reset();
//        break;
//      case ' ':
//        resetCamera();
//        break;
//      case 'p':
//      case 'P':
//        is_paused = !is_paused;
//        break;
//      case 'n':
//      case 'N':
//        if (is_paused) {
//          is_paused = false;
//          drawContents();
//          is_paused = true;
//        }
//        break;
//    }
//  }
//
//  return true;
//}
//
//bool SnowSimulator::dropCallbackEvent(int count, const char **filenames) {
//  return true;
//}
//
//bool SnowSimulator::scrollCallbackEvent(double x, double y) {
//  camera.move_forward(y * scroll_rate);
//  return true;
//}
//
//bool SnowSimulator::resizeCallbackEvent(int width, int height) {
//  screen_w = width;
//  screen_h = height;
//
//  camera.set_screen_size(screen_w, screen_h);
//  return true;
//}
