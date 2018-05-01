#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <glm/glm.hpp>
#include "interpolation.h"

class Particle {
public:
  Particle(glm::vec3 position, float mass): 
    position(position), mass(mass), velocity(glm::vec3(0)) {};

  glm::vec3 position;
  glm::vec3 velocity;
  float mass;
  float volume;

  glm::mat3 deformation_grad_E;
  glm::mat3 deformation_grad_P;

  // for step 3
  glm::mat3 F_hat_Ep;

  // Grid neighborhood bounds (compute once per simulation step)
  int i_lo;
  int i_hi;
  int j_lo;
  int j_hi;
  int k_lo;
  int k_hi;

  /* Memoize b-spline and b-spline gradient
   * maximum neighborhood size is 4, so we allocate this amount
   * but we might not use all 4x4x4 values
  */
  float b_spline_val[4][4][4];
  glm::vec3 b_spline_grad_val[4][4][4];

  void compute_neighborhood_bounds(int dim_x, int dim_y, int dim_z, float h) {
    i_lo = std::max((int) ceil(position.x / h - 2), 0);
    i_hi = std::min((int) floor(position.x / h + 2) + 1, (int) dim_x);
    j_lo = std::max((int) ceil(position.y / h - 2), 0);
    j_hi = std::min((int) floor(position.y / h + 2) + 1, (int) dim_y);
    k_lo = std::max((int) ceil(position.z / h - 2), 0);
    k_hi = std::min((int) floor(position.z / h + 2) + 1, (int) dim_z);
  }

  void compute_b_spline_grad(float h) {
    for (int dest_i = i_lo; dest_i < i_hi; ++dest_i) {
      for (int dest_j = j_lo; dest_j < j_hi; ++dest_j) {
        for (int dest_k = k_lo; dest_k < k_hi; ++dest_k) {
          glm::vec3 scaled = position / h - glm::vec3(dest_i, dest_j, dest_k);
          b_spline_val[dest_i - i_lo][dest_j - j_lo][dest_k - k_lo] = b_spline(scaled);
          b_spline_grad_val[dest_i - i_lo][dest_j - j_lo][dest_k - k_lo] = b_spline_grad(scaled, h);
        }
      }
    }
  }

  float b_spline_at(int grid_i, int grid_j, int grid_k) {
    return b_spline_val[grid_i - i_lo][grid_j - j_lo][grid_k - k_lo];
  }

  glm::vec3 b_spline_grad_at(int grid_i, int grid_j, int grid_k) {
    return b_spline_grad_val[grid_i - i_lo][grid_j - j_lo][grid_k - k_lo];
  }
};


///////////////////////////////////////////////////////////////////

#endif /* PARTICLE_H */