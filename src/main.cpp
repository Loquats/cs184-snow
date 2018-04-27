
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <glm/vec3.hpp>

#include <iostream>
#include <vector>
#include "glm/ext.hpp"
#include "particle.h"
#include "grid.h"
#include "force.h"
#include "camera.h"
#include "snowsim.h"
#include "shader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLFWwindow *window = nullptr;

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


  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
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

int main(void)
{

  loadOpenGl();

  setGlfwCallbacks();

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


  // NOTE: OpenGL error checks have been omitted for brevity
  printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
  printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
  glGetString(GL_SHADING_LANGUAGE_VERSION));

  glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

  // test_particle();
  // test_grid_node();

  size_t dim_x = 10;
  size_t dim_y = 11;
  size_t dim_z = 12;
  Grid* grid = new Grid(dim_x, dim_y, dim_z, 1.0);

  SnowSimulator *snowsim = new SnowSimulator();
  snowsim->loadGrid(grid);
  //todo define object schemas and shit and find way to load
//  snowsim->loadCollisionObjects(&objects);


  // print_grid_node(grid->nodes[dim_x-1][dim_y-1][dim_z-1]);

  // Put a test particle
//  Particle* a_particle = new Particle(glm::vec3(4.5, 5.5, 6.5), 10);
//  a_particle->velocity = glm::vec3(1.0, 2.0, 3.0);
//  grid->nodes[4][5][6]->particles.push_back(a_particle);
  // cout << grid->nodes.size() << " " << grid->nodes[0].size() << " " << grid->nodes[0][0].size() << "\n";
  // cout << grid->nodes[4][5][6]->particles.size() << "\n";
  // cout << grid->nodes[4][5][7]->particles.size() << "\n";

  float delta_t = 0.0001; // todo: move these constants elsewhere
  float mu_0 = 1.0;
  float lambda_0 = 1.0;
  float xi = 10;
  particle_to_grid(grid);
  compute_particle_volumes(grid);
  compute_F_hat_Ep(grid, delta_t);
  compute_grid_forces(grid, mu_0, lambda_0, xi);

  for (int i = 0; i < grid->nodes.size(); ++i) {
    for (int j = 0; j < grid->nodes[i].size(); ++j) {
      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
        Particle* a_particle = new Particle(glm::vec3(float(i) + 0.5, float(j) + 0.5, float(k) + 0.5), 10);
        a_particle->velocity = glm::vec3(1.0, 2.0, 3.0);
        grid->nodes[i][j][k]->particles.push_back(a_particle);
      }
    }
  }

  Shader baseshader("../src/shaders/camera.vs", "../src/shaders/simple.fs");
  snowsim->init(&camera, &baseshader);
//  int num_particles = int(dim_x) * int(dim_y) * int(dim_z) + 1;
//  MatrixXd vertices(num_particles, 3);
//  int z = 0;
//  for (int i = 0; i < grid->nodes.size(); ++i) {
//    for (int j = 0; j < grid->nodes[i].size(); ++j) {
//      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
//        // cout << grid->nodes[4][5][6]->particles.size() << " grid " << i << " " << j << " " << k << " " << grid->nodes[i][j][k]->particles.size() << "\n";
//        for (Particle *particle : grid->nodes[i][j][k]->particles) {  // key location
//          glm::vec3 pos = particle->position;
//          vertices(z,0) = pos.x;
//          vertices(z,1) = pos.y;
//          vertices(z,2) = pos.z;
//          z += 1;
//        }
//      }
//    }
//  }
//
//  cout << vertices;

  float vertices[] = {
          0.5f,  0.5f, 0.0f,  // top right
          0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f,  // bottom left
  };



  while (!glfwWindowShouldClose(window))
    {
      cameraInputTick();

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      snowsim->drawContents();

      glfwPollEvents();

      glfwSwapBuffers(window);

    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}