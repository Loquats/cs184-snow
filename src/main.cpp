
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


int SCR_WIDTH = 1200;
int SCR_HEIGHT = 800;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLFWwindow *window = nullptr;

SnowSimulator *snowsim;

using namespace std;

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
  int length = 30;

  // fps wanted
  int frames_per_second = 30;

  if (argc == 1) { // No arguments, default initialization
    outFilename = "out.avi";
    // default 4 seconds at 30 fps
    length = 120;
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

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  n_func_init();
  n_func_derivative_init();


  // NOTE: OpenGL error checks have been omitted for brevity
  printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
  printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
  glGetString(GL_SHADING_LANGUAGE_VERSION));

  glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

  size_t dim_x = 4;
  size_t dim_y = 5;
  size_t dim_z = 6;
  float h = 1.0;
  Grid* grid = new Grid(dim_x, dim_y, dim_z, h);

  snowsim = new SnowSimulator(frames_per_second, length);
  snowsim->loadGrid(grid);

  //todo define object schemas and shit and find way to load
  int num_particles = 100;
  float radius = 2.0;
  createSphereUniformParticles(grid, num_particles, radius);
  vector<CollisionObject *> objects;

  // Make the ground plane
  Shader baseshader("../src/shaders/camera.vert", "../src/shaders/simple.frag");
  vec3 grid_dim(dim_x, dim_y, dim_z);
  vec3 point(0.0f, -1.0f * dim_y * h / 2, 0.0f);
  vec3 normal(0.0f, 1.0f, 0.0f);
  Plane *p = new Plane(point, normal, grid_dim, 0.5);
  objects.push_back(p);

  // make the wedge
  glm::mat4 model;
  model = glm::translate(model, glm::vec3(-float(grid_dim.x)/2, -float(grid_dim.y)/2, -float(grid_dim.z)/2));
  vec3 corner(2, 2, 1);
  vec3 top_edge(0, 0, 4);
  vec3 edge1(1, -1, 0);
  vec3 edge2(-1, -1, 0);
  float mu = 0.5;
  Rectangle* wedge_rect1 = new Rectangle(corner, top_edge, edge1, mu, model);
  Rectangle* wedge_rect2 = new Rectangle(corner, top_edge, edge2, mu, model);
  objects.push_back(wedge_rect1);
  objects.push_back(wedge_rect2);

  glm::vec4 hot_pink(1.0f, 0.5f, 1.0f, 1.0f);
  baseshader.use();
  baseshader.setVec4("in_color", hot_pink);
  snowsim->init(&camera, &baseshader);
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}