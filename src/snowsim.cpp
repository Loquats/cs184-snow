// inspired by /copied from clothsim :)

#include <cmath>
#include <glad/glad.h>

//#include <CGL/vector3D.h>
//#include <nanogui/nanogui.h>

#include "snowsim.h"

#include "camera.h"
#include "grid.h"
#include "collision/plane.h"
#include "collision/sphere.h"
//#include "misc/camera_info.h"
#include "glm/ext.hpp"

using namespace std;

SnowSimulator::SnowSimulator() {

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
void SnowSimulator::init(Camera *camera, Shader *shader) {
  this->camera = camera;
  this->shader = shader;
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
      // lines from y=0 to y=dim_y at z=0
      glm::vec3 bottom(float(x_coord), 0.0f, float(z_coord));
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

  GLfloat cube_vertices[] = {
          -0.05, -0.05, -0.05,   -0.05, -0.05,  0.05,   -0.05,  0.05,  0.05,   -0.05,  0.05, -0.05,
          0.05, -0.05, -0.05,    0.05, -0.05,  0.05,    0.05,  0.05,  0.05,    0.05,  0.05, -0.05,
          -0.05, -0.05, -0.05,   -0.05, -0.05,  0.05,    0.05, -0.05,  0.05,    0.05, -0.05, -0.05,
          -0.05,  0.05, -0.05,   -0.05,  0.05,  0.05,    0.05,  0.05,  0.05,    0.05,  0.05, -0.05,
          -0.05, -0.05, -0.05,   -0.05,  0.05, -0.05,    0.05,  0.05, -0.05,    0.05, -0.05, -0.05,
          -0.05, -0.05,  0.05,   -0.05,  0.05,  0.05,    0.05,  0.05,  0.05,    0.05, -0.05,  0.05
  };

  glGenVertexArrays(1, &particle_VAO);
  glGenBuffers(1, &particle_VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(particle_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, particle_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);


  // Initialize GUI
//  initGUI(screen);
//  screen->setSize(default_window_size);

//  // Initialize camera
//
//  CGL::Collada::CameraInfo camera_info;
//  camera_info.hFov = 50;
//  camera_info.vFov = 35;
//  camera_info.nClip = 0.01;
//  camera_info.fClip = 10000;
//
//  // Try to intelligently figure out the camera target
//
//  Vector3D avg_pm_position(0, 0, 0);
//
//  for (auto &pm : cloth->point_masses) {
//    avg_pm_position += pm.position / cloth->point_masses.size();
//  }
//
//  CGL::Vector3D target(avg_pm_position.x, avg_pm_position.y / 2,
//                       avg_pm_position.z);
//  CGL::Vector3D c_dir(0., 0., 0.);
//  canonical_view_distance = max(cloth->width, cloth->height) * 0.9;
//  scroll_rate = canonical_view_distance / 10;
//
//  view_distance = canonical_view_distance * 2;
//  min_view_distance = canonical_view_distance / 10.0;
//  max_view_distance = canonical_view_distance * 20.0;
//
//  // canonicalCamera is a copy used for view resets
//
//  camera.place(target, acos(c_dir.y), atan2(c_dir.x, c_dir.z), view_distance,
//               min_view_distance, max_view_distance);
//  canonicalCamera.place(target, acos(c_dir.y), atan2(c_dir.x, c_dir.z),
//                        view_distance, min_view_distance, max_view_distance);
//
//  screen_w = default_window_size(0);
//  screen_h = default_window_size(1);
//
//  camera.configure(camera_info, screen_w, screen_h);
//  canonicalCamera.configure(camera_info, screen_w, screen_h);
}


void SnowSimulator::drawContents() {

  // TODO
  glEnable(GL_DEPTH_TEST);

  if (!is_paused) {
    vector<vec3> external_accelerations = {gravity};

    for (int i = 0; i < simulation_steps; i++) {
//      grid->simulate(frames_per_sec, simulation_steps, external_accelerations, collision_objects);
    }
  }

  shader->use();
  // pass projection matrix to shader (note that in this case it could change every frame)
  glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  shader->setMat4("projection", projection);

  // camera/view transformation
  glm::mat4 view = camera->GetViewMatrix();
  shader->setMat4("view", view);
  drawGrid();
  drawParticles();
//
//  for (CollisionObject *co : *collision_objects) {
//    co->render(shader);
//  }
}

void SnowSimulator::drawGrid() {
  glBindVertexArray(grid_VAO);
  glm::mat4 model;
  model = glm::translate(model, glm::vec3(-float(grid->dim_x)/2, -float(grid->dim_y)/2, -float(grid->dim_z)/2));
  shader->setMat4("model", model);
  int num_vertices = (grid->dim_x + 1) * (grid->dim_y + 1) * (grid->dim_z + 1) * 2;

  glDrawArrays(GL_LINES, 0, num_vertices * 3);
}

void SnowSimulator::drawParticles() {
  vector<Particle *> all_particles;
  for (int i = 0; i < grid->nodes.size(); ++i) {
    for (int j = 0; j < grid->nodes[i].size(); ++j) {
      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
        for (Particle *particle : grid->nodes[i][j][k]->particles) {
          all_particles.push_back(particle);
        }
      }
    }
  }
  for(Particle* particle : all_particles) {
    glm::mat4 model;
    model = glm::translate(model, particle->position);
    model = glm::translate(model, glm::vec3(-float(grid->dim_x) / 2, -float(grid->dim_y) / 2, -float(grid->dim_z) / 2));
//    model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.3f, 0.5f));
    shader->setMat4("model", model);
    glBindVertexArray(particle_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 24);
  }

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
