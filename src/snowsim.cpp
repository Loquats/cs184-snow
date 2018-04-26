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
  // pass projection matrix to shader (note that in this case it could change every frame)
  glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  shader->setMat4("projection", projection);

  // camera/view transformation
  glm::mat4 view = camera->GetViewMatrix();
  shader->setMat4("view", view);

  // TODO
  glEnable(GL_DEPTH_TEST);

  if (!is_paused) {
    vector<vec3> external_accelerations = {gravity};

    for (int i = 0; i < simulation_steps; i++) {
//      grid->simulate(frames_per_sec, simulation_steps, external_accelerations, collision_objects);
    }
  }
//
//  // Bind the active shader
//
//  GLShader shader = shaders[activeShader];
//  shader.bind();
//
//  // Prepare the camera projection matrix
//
//  Matrix4f model;
//  model.setIdentity();
//
//  Matrix4f view = getViewMatrix();
//  Matrix4f projection = getProjectionMatrix();
//
//  Matrix4f viewProjection = projection * view;
//
//  shader.setUniform("model", model);
//  shader.setUniform("viewProjection", viewProjection);
//
//  switch (activeShader) {
//    case WIREFRAME:
//      drawWireframe(shader);
//      break;
//    case NORMALS:
//      drawNormals(shader);
//      break;
//    case PHONG:
//      drawPhong(shader);
//      break;
//  }
//
//  for (CollisionObject *co : *collision_objects) {
//    co->render(shader);
//  }
}

void SnowSimulator::drawGrid() {
}

void SnowSimulator::drawParticles() {


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
