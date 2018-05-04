
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <glm/vec3.hpp>

#include <iostream>
#include <vector>
#include <getopt.h>
#include "particle.h"
#include "grid.h"
#include "force.h"
#include "camera.h"
#include "snowsim.h"
#include "collision/plane.h"
#include "collision/rectangle.h"
#include "misc/sampling.h"

using namespace std;

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f, 3.0f, 70.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLFWwindow *window = nullptr;

SnowSimulator *snowsim;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
  SCR_WIDTH = width;
  SCR_HEIGHT = height;
}


void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}


void loadOpenGl() {
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwSetErrorCallback(error_callback);
  glfwSetTime(0);

  // initialize glfw with version 4.1
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simple example", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    std::cout << "Failed to create GLFW window" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);


  int gladInitRes = gladLoadGL();
  if (!gladInitRes) {
    fprintf(stderr, "Unable to initialize glad\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  }

  glfwSwapInterval(1);
  glfwSwapBuffers(window);
}

void setGlfwCallbacks() {
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
}

void cameraInputTick() {
  // per-frame time logic
  // --------------------
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  processInput(window);
}

int main(int argc, char **argv)
{
  // renders to a file instead of displaying.
  // ONLY USE THESE COMMAND LINE ARGS IF IN HEADLESS MODE.  OTHERWISE FPS AND LENGTH WILL JUST TERMINATE
  // THE PROGRAM AFTER A CERTAIN NUMBER OF FRAMES ARE CREATED
  bool headless = false;
  string outFilename = "out.avi";
  // numframes in output
  int length = 12000;

  // fps wanted
  int frames_per_second = 30;

  if (argc == 1) { // No arguments, default initialization
    outFilename = "out.avi";
    // default 4 seconds at 30 fps
    length = 12000;
    frames_per_second = 30;
//    loadObjectsFromFile(default_file_name, &cloth, &cp, &objects);
  } else {
    int c;

    while ((c = getopt (argc, argv, "o:l:n:h")) != -1) {
      switch (c) {
        case 'h':
          headless = true;
          break;
        case 'o':
          outFilename = string(optarg);
          break;
        case 'l':
          length = atoi(optarg);
          break;
        case 'n':
          frames_per_second = atoi(optarg);
          break;
        default:
          cout << optopt;
          break;
      }
    }
  }

  loadOpenGl();

  setGlfwCallbacks();

//  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  n_func_init();
  n_func_derivative_init();


  // NOTE: OpenGL error checks have been omitted for brevity
  printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
  printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
  glGetString(GL_SHADING_LANGUAGE_VERSION));

  glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

  int dim_x = 60;
  int dim_y = 60;
  int dim_z = 60;
  vec3 dim(dim_x, dim_y, dim_z);
  float h = 1.0;

  glm::mat4 worldtomodel;
  worldtomodel = glm::translate(worldtomodel, glm::vec3(dim.x / 2, dim.y / 2, dim.z / 2));
  glm::mat4 modeltoworld;
  modeltoworld = glm::translate(modeltoworld, glm::vec3(-dim.x / 2, -dim.y / 2, -dim.z / 2));

  Grid* grid = new Grid(dim_x, dim_y, dim_z, h);

  snowsim = new SnowSimulator(frames_per_second, length);
  snowsim->loadGrid(grid);

  //todo define object schemas and shit and find way to load
  int num_particles = 1000;
  float radius = float(dim_x) / 6;
  createSphereUniformParticles(grid, num_particles, radius);
  // createTwoParticles(grid);
  vector<CollisionObject *> objects;

  Shader baseshader("../src/shaders/camera.vert", "../src/shaders/simple.frag");
  glm::vec4 pink(1.0f, 0.5f, 1.0f, 1.0f);
  // Make the ground plane
  vec4 ground_color = vec4(0.3f, 0.3f, 0.3f, 1.0f);
  vec3 origin(-dim.x/2.0, -dim.y/2.0, -dim.z/2.0);
  vec3 axis_x(dim_x, 0, 0);
  vec3 axis_y(0, dim_y, 0);
  vec3 axis_z(0, 0, dim_z);
  Rectangle* ground_rect = new Rectangle(origin, axis_x, axis_z, 0.2, modeltoworld, worldtomodel, ground_color);
  objects.push_back(ground_rect);

  vec4 transparent = vec4(0, 0, 0, 0);
  vec3 o_axis_x = origin + axis_x;
  vec3 o_axis_z = origin + axis_z;
  Rectangle* wall_rect1 = new Rectangle(origin, axis_x, axis_y, 0.2, modeltoworld, worldtomodel, transparent);
  Rectangle* wall_rect2 = new Rectangle(origin, axis_y, axis_z, 0.2, modeltoworld, worldtomodel, transparent);
  Rectangle* wall_rect3 = new Rectangle(o_axis_x, axis_y, axis_z, 0.2, modeltoworld, worldtomodel, transparent);
  Rectangle* wall_rect4 = new Rectangle(o_axis_z, axis_x, axis_y, 0.2, modeltoworld, worldtomodel, transparent);
  objects.push_back(wall_rect1);
  objects.push_back(wall_rect2);
  objects.push_back(wall_rect3);
  objects.push_back(wall_rect4);

  // Make the wedge
  vec4 wedge_color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
  vec3 corner(0,-0.05*dim.y,-dim.z/2);
  vec3 top_edge(0, 0, 0.8 * dim_z);
  vec3 edge1(0.15*dim_x, -0.15*dim_y, 0);
  vec3 edge2(-0.15*dim_x, -0.15*dim_y, 0);
  float mu = 0.2;
  Rectangle* wedge_rect1 = new Rectangle(corner, top_edge, edge1, mu, modeltoworld, worldtomodel, wedge_color);
  Rectangle* wedge_rect2 = new Rectangle(corner, top_edge, edge2, mu, modeltoworld, worldtomodel, wedge_color);
  objects.push_back(wedge_rect1);
  objects.push_back(wedge_rect2);

  baseshader.use();
  snowsim->init(&camera, &baseshader, modeltoworld);
  snowsim->loadCollisionObjects(&objects);

  int frame_counter = 0;
  char command[400];
  float *data;
  FILE *ffmpeg;

  int w_ = 0;
  int h_ = 0;
  glfwGetFramebufferSize(window, &w_, &h_);

  if (headless) {
    snprintf(command, sizeof(command), "ffmpeg -vcodec rawvideo -f rawvideo -pix_fmt rgba -s %dx%d -i pipe: -vf vflip -vcodec h264 -r %d %s", w_, h_, frames_per_second, outFilename.c_str());
    ffmpeg = popen(command, "w");
    data = (float *)malloc(sizeof(float) * 4 * w_ * h_);
  }

  while (!glfwWindowShouldClose(window) and frame_counter < length)
    {
      cout << "Iteration " << frame_counter << " ";
      cameraInputTick();

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      snowsim->drawContents();

      glfwPollEvents();


      if(headless) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
          cerr << "OpenGL error: " << err << endl;
        }
        glReadPixels(0, 0, w_, h_, GL_RGBA, GL_UNSIGNED_BYTE, data);
        while ((err = glGetError()) != GL_NO_ERROR) {
          cerr << "OpenGL error: " << err << endl;
        }
        fwrite(data, w_ * h_ * 4, 1, ffmpeg);
      }

      glfwSwapBuffers(window);

      frame_counter++;
    }
    if(headless) {
      pclose(ffmpeg);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}