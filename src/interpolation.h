#include <glm/glm.hpp>

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

float b_spline(glm::vec3 particle_pos, glm::vec3 grid_index, float h) {
	// ie. particle_pos / h - grid_index
	glm::vec3 scaled = (particle_pos - h * grid_index) / h;
	return N_func(scaled.x) * N_func(scaled.y) * N_func(scaled.z);
}