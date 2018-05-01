#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <glm/glm.hpp>

float sgn(float x);

float N_func(float x);

float N_func_derivative(float x);

float b_spline(glm::vec3 scaled);

glm::vec3 b_spline_components(glm::vec3 scaled);

glm::vec3 b_spline_grad(glm::vec3 scaled, float h);

void n_func_init();

void n_func_derivative_init();

#endif /* INTERPOLATION_H */