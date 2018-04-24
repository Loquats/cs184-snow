
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

    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}