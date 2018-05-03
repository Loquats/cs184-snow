
#ifndef SNOW_SNOWSIM_H
#define SNOW_SNOWSIM_H


// inspired by /copied from clothsim :)

#include <stdbool.h>
#include "grid.h"
#include "collision/collisionObject.h"
#include "camera.h"
#include "shader.h"

#include <Eigen/Dense>
using Eigen::MatrixXd;

using namespace glm;

class SnowSimulator {
public:
  SnowSimulator(int frames_per_sec, int length);
  ~SnowSimulator();

  void init(Camera *camera, Shader *shader, glm::mat4 model);

  void loadGrid(struct Grid *grid);
//  void loadClothParameters(ClothParameters *cp);
  void loadCollisionObjects(std::vector<CollisionObject *> *objects);
  virtual void drawContents();

  // Screen events

//  virtual bool cursorPosCallbackEvent(double x, double y);
//  virtual bool mouseButtonCallbackEvent(int button, int action, int modifiers);
//  virtual bool keyCallbackEvent(int key, int scancode, int action, int mods);
//  virtual bool dropCallbackEvent(int count, const char **filenames);
//  virtual bool scrollCallbackEvent(double x, double y);
//  virtual bool resizeCallbackEvent(int width, int height);

private:
  void drawGrid(vec4 color);
  void drawParticles(vec4 color);
  void drawGridForces();
  void drawParticleForces();

  // Camera methods

//  virtual void resetCamera();
//  virtual MatrixXd getProjectionMatrix();
//  virtual MatrixXd getViewMatrix();

  // Default simulation values

  int frames_per_sec = 30;
  int length;
  float delta_t = 1e-3;

  glm::mat4 modeltoworld;

  glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);

  Grid *grid;
//  ClothParameters *cp;
  vector<CollisionObject *> *collision_objects;

  Shader *shader;
  unsigned int grid_VAO, grid_VBO;
  unsigned int particle_VAO, particle_VBO;
  unsigned int grid_force_VAO, grid_force_VBO;
  unsigned int particle_velocity_VAO, particle_velocity_VBO;

  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;

  // Camera attributes

  Camera *camera;
//  CGL::Camera canonicalCamera;

  double view_distance;
  double canonical_view_distance;
  double min_view_distance;
  double max_view_distance;

  double scroll_rate;

  // Screen methods

  void mouseLeftDragged(double x, double y);
  void mouseRightDragged(double x, double y);
  void mouseMoved(double x, double y);

  // Mouse flags

  bool left_down = false;
  bool right_down = false;
  bool middle_down = false;

  // Keyboard flags

  bool ctrl_down = false;

  // Simulation flags

  bool is_paused = false;

  // Screen attributes

  int mouse_x;
  int mouse_y;

  int screen_w;
  int screen_h;

//  bool is_alive = true;

//  Vector2i default_window_size = Vector2i(1024, 800);
};

#endif //SNOW_SNOWSIM_H
