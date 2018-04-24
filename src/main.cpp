
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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


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
          "uniform mat4 view;\n"
          "uniform mat4 projection;"
          "void main()\n"
          "{\n"
          "   gl_PointSize = 10.0;\n"
          "   gl_Position = projection * view * vec4(aPos, 1.0f);\n"
          "}\0";
  const char *fragmentShaderSource = "#version 330 core\n"
          "out vec4 FragColor;\n"
          "void main()\n"
          "{\n"
          "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
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
    Grid* grid = new Grid(dim_x, dim_y, dim_z, 1.0);
    // print_grid_node(grid->nodes[dim_x-1][dim_y-1][dim_z-1]);

    // Put a test particle
    Particle* a_particle = new Particle(glm::vec3(4.5, 5.5, 6.5), 10);
    a_particle->velocity = glm::vec3(1.0, 2.0, 3.0);
    grid->nodes[4][5][6]->particles.push_back(a_particle);
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

    // test_glm_to_eigen();
    // test_eigen_to_glm();
    // cout << grid->nodes[4][5][6]->particles[0]->mass << "\n";
    // cout << grid->nodes[4][5][6]->particles[0]->volume << "\n";
    // cout << glm::l2Norm(grid->nodes[4][5][6]->force) << "\n";
    for (int i = 0; i < 10; ++i) {
        for (int j = 3; j < 9; ++j) {
            for (int k = 4; k < 10; k++) {
                // cout << glm::to_string(grid[i][j][k]->velocity) << " ";
                // cout << grid->nodes[i][j][k]->mass << " ";
                cout << glm::l2Norm(grid->nodes[i][j][k]->force) << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

  int shader_program = init_shaders();


  // float vertices[] = {
  //         0.5f,  0.5f, 0.0f,  // top right
  //         0.5f, -0.5f, 0.0f,  // bottom right
  //         -0.5f, -0.5f, 0.0f,  // bottom left
  //         -0.5f,  0.5f, 0.9f   // top left
  // };
  float vertices[50*3];
  for (int i = 0; i < 50*3; ++i) {
    float r = (float) rand() / (float) RAND_MAX;
    vertices[i] = r - 0.5;
  }

  // from learnopengl tutorial https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.1.hello_triangle/hello_triangle.cpp
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

      glClearColor(0.4f, 0.6f, 0.8f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);


      glUseProgram(shader_program);

      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]);

      // camera/view transformation
      glm::mat4 view = camera.GetViewMatrix();
      glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, &view[0][0]);

      glBindVertexArray(VAO);
      glDrawArrays(GL_POINTS, 0, 50);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}