
#ifndef SNOW_SNOWSIM_H
#define SNOW_SNOWSIM_H


// inspired by /copied from clothsim :)

#include <stdbool.h>
#include "grid.h"
#include "collision/collisionObject.h"

#include <Eigen/Dense>
using Eigen::MatrixXd;

class SnowSimulator {
public:
  SnowSimulator();
  ~SnowSimulator();

  void init();

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
  void drawGrid();
  void drawParticles();

  // Camera methods

//  virtual void resetCamera();
//  virtual MatrixXd getProjectionMatrix();
//  virtual MatrixXd getViewMatrix();

  // Default simulation values

//  int frames_per_sec = 90;
//  int simulation_steps = 30;


  Grid *grid;
//  ClothParameters *cp;
  vector<CollisionObject *> *collision_objects;


  // Camera attributes

//  CGL::Camera camera;
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
