
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


using namespace std;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static int init_shaders() {
  const char *vertexShaderSource = "#version 330 core\n"
          "layout (location = 0) in vec3 aPos;\n"
          "void main()\n"
          "{\n"
          "   gl_PointSize = 10.0;\n"
          "   gl_Position = vec4(aPos, 1.0);\n"
          "}\0";
  const char *fragmentShaderSource = "#version 330 core\n"
          "out vec4 FragColor;\n"
          "void main()\n"
          "{\n"
          "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
          "}\n\0";
  // build and compile our shader program
  // ------------------------------------
  // vertex shader
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // fragment shader
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // link shaders
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glUseProgram(shaderProgram);
  return shaderProgram;
}
int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    int gladInitRes = gladLoadGL();
    if (!gladInitRes) {
      fprintf(stderr, "Unable to initialize glad\n");
      glfwDestroyWindow(window);
      glfwTerminate();
      return -1;
    }
    glfwSwapInterval(1);
    // NOTE: OpenGL error checks have been omitted for brevity
    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
    glGetString(GL_SHADING_LANGUAGE_VERSION));

    // test_particle();
    // test_grid_node();

    size_t dim_x = 10;
    size_t dim_y = 11;
    size_t dim_z = 12;
    // Grid grid = init_grid(dim_x, dim_y, dim_z);
    Grid* grid = new Grid(dim_x, dim_y, dim_z, 1.0);
    // cout << grid->h << "\n";
    // print_grid_node(grid->nodes[dim_x-1][dim_y-1][dim_z-1]);

    // Put a test particle
    Particle* a_particle = new Particle(glm::vec3(4.5, 5.5, 6.5), 10);
    a_particle->velocity = glm::vec3(1.0, 2.0, 3.0);
    vector<vector<vector<GridNode *> > > bla = grid->nodes;



    particle_to_grid(grid);
    compute_particle_volumes(grid);



  int shader_program = init_shaders();


  float vertices[] = {
          0.5f,  0.5f, 0.0f,  // top right
          0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f,  // bottom left
          -0.5f,  0.5f, 0.0f   // top left
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
  glEnable(GL_PROGRAM_POINT_SIZE);

  while (!glfwWindowShouldClose(window))
    {

      glClearColor(0.1f, 0.3f, 0.9f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);


      glUseProgram(shader_program);
      glBindVertexArray(VAO);
      glDrawArrays(GL_POINTS, 0, 3);


//      glBindVertexArray(VAO);
//      glDrawArrays(GL_POINTS, 0, 4);
//
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}