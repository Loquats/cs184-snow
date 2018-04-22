#include <glm/glm.hpp>

/* sign function: https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
*/ 
float sgn(float x) {
	return (x > 0) - (x < 0);
}

float N_func(float x) {
	float abs_x = abs(x);
	if (abs_x < 1) {
		return 0.5 * pow(abs_x, 3) - pow(x, 2) + 2.0 / 3.0;
	} else if (abs_x < 2) {
		return -1.0 / 6.0 * pow(abs_x, 3) + pow(x, 2) - 2.0 * abs_x + 4.0 / 3.0;
	} else {
		return 0;
	}
}

float N_func_derivative(float x) {
	float abs_x = abs(x);
	if (abs_x < 1) {
		return 1.5 * x * x * sgn(x) - 2 * x;
	} else if (abs_x < 2) {
		return -0.5 * x * x * sgn(x) + 2 * x - 2 * sgn(x);
	} else {
		return 0;
	}
}

float b_spline(glm::vec3 particle_pos, glm::vec3 grid_index, float h) {
	// ie. particle_pos / h - grid_index
	glm::vec3 scaled = (particle_pos - h * grid_index) / h;
	return N_func(scaled.x) * N_func(scaled.y) * N_func(scaled.z);
}

glm::vec3 b_spline_components(glm::vec3 particle_pos, glm::vec3 grid_index, float h) {
	glm::vec3 scaled = (particle_pos - h * grid_index) / h;
	return glm::vec3(N_func(scaled.x), N_func(scaled.y), N_func(scaled.z));
}

/*
* Using that motherfuckin chain rule, ye
*/
glm::vec3 b_spline_grad(glm::vec3 particle_pos, glm::vec3 grid_index, float h) {
	glm::vec3 scaled = (particle_pos - h * grid_index) / h;
	glm::vec3 components = b_spline_components(particle_pos, grid_index, h);
	float dx = components.y * components.z * N_func_derivative(scaled.x) / h;
	float dy = components.x * components.z * N_func_derivative(scaled.y) / h;
	float dz = components.x * components.y * N_func_derivative(scaled.z) / h;
	return glm::vec3(dx, dy , dz);
}
