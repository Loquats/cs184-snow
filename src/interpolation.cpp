#include <glm/glm.hpp>
#include "interpolation.h"

const int NFUNCRESOLUTION = 10000;
const float NFUNCSPACING = 4.0f/NFUNCRESOLUTION;
static float n_func[NFUNCRESOLUTION] = {0};
static float n_func_derivative[NFUNCRESOLUTION] = {0};

/* sign function: https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
*/ 
float sgn(float x) {
	return (x > 0) - (x < 0);
}

float N_func(float x) {
	float abs_x = abs(x);
	if (abs_x < 1) {
		return 0.5 * abs_x * abs_x * abs_x - x * x + 2.0 / 3.0;
	} else if (abs_x < 2) {
		return -1.0 / 6.0 * abs_x * abs_x * abs_x + x * x - 2.0 * abs_x + 4.0 / 3.0;
	} else {
		return 0;
	}
}

float N_func_derivative(float x) {
	float abs_x = abs(x);
	float sign = sgn(x);
	if (abs_x < 1) {
		return 1.5 * x * x * sign - 2 * x;
	} else if (abs_x < 2) {
		return -0.5 * x * x * sign + 2 * x - 2 * sign;
	} else {
		return 0;
	}
}

void n_func_init() {
	//precompute nfunc
	for (int i = 0; i < NFUNCRESOLUTION; i++) {
		n_func[i] = N_func(((i+0.5) * NFUNCSPACING) - 2);
	}
}

float inline n_func_memoized(float x) {
	if (x < 2 or x > 2) {
		return 0;
	}
	int idx = int(floor((x+2)/NFUNCSPACING));
	return n_func[idx];
}

void n_func_derivative_init() {
	//precompute nfunc
	for (int i = 0; i < NFUNCRESOLUTION; i++) {
		n_func_derivative[i] = N_func_derivative(((i+0.5) * NFUNCSPACING) - 2);
	}
}

float inline n_func_derivative_memoized(float x) {
	if (x < 2 or x > 2) {
		return 0;
	}
	int idx = int(floor((x+2)/NFUNCSPACING));
	return n_func_derivative[idx];
}

float b_spline(glm::vec3 scaled) {
	return n_func_memoized(scaled.x) * n_func_memoized(scaled.y) * n_func_memoized(scaled.z);
}

glm::vec3 b_spline_components(glm::vec3 scaled) {
	return glm::vec3(n_func_memoized(scaled.x), n_func_memoized(scaled.y), n_func_memoized(scaled.z));
}

/*
* Using that motherfuckin chain rule, ye
*/
glm::vec3 b_spline_grad(glm::vec3 scaled, float h) {
	glm::vec3 components = b_spline_components(scaled);
	float dx = components.y * components.z * n_func_derivative_memoized(scaled.x) / h;
	float dy = components.x * components.z * n_func_derivative_memoized(scaled.y) / h;
	float dz = components.x * components.y * n_func_derivative_memoized(scaled.z) / h;
	return glm::vec3(dx, dy , dz);
}