#ifndef CGL_UTIL_SPHEREDRAWING_H
#define CGL_UTIL_SPHEREDRAWING_H


#include <glm/vec3.hpp>

using namespace glm;


/**
 * Draws a sphere with the given position and radius in opengl, using the
 * current modelview/projection matrices and color/material settings.
 */
void draw_sphere(const vec3 &p, double r);


#endif // CGL_UTIL_SPHEREDRAWING_H
