#include <algorithm>
#include <iostream>
#include <glm/ext.hpp>
#include "grid.h"
#include "particle.h"
#include "interpolation.h"
#include "force.h"
#include <glm/glm.hpp>
#include <Eigen/SVD>

using namespace Eigen;
/*
* Step 1: compute the mass and velocity of each grid node based the particles
* in its neighborhood
*/
void particle_to_grid(Grid* grid) {
  for (int i = 0; i < grid->nodes.size(); ++i) {
    for (int j = 0; j < grid->nodes[i].size(); ++j) {
      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
        // cout << grid->nodes[4][5][6]->particles.size() << " grid " << i << " " << j << " " << k << " " << grid->nodes[i][j][k]->particles.size() << "\n";
        for (Particle* particle : grid->nodes[i][j][k]->particles) {	// key location
          // cout << "particle\n";
          glm::vec3 pos = particle->position;
          int i_lo = max((int) ceil(pos.x / grid->h - 2), 0);
          int i_hi = min((int) floor(pos.x / grid->h + 2) + 1, (int) grid->nodes.size());
          int j_lo = max((int) ceil(pos.y / grid->h - 2), 0);
          int j_hi = min((int) floor(pos.y / grid->h + 2) + 1, (int) grid->nodes[i].size());
          int k_lo = max((int) ceil(pos.z / grid->h - 2), 0);
          int k_hi = min((int) floor(pos.z / grid->h + 2) + 1, (int) grid->nodes[i][j].size());
          for (int dest_i = i_lo; dest_i < i_hi; ++dest_i) {
            for (int dest_j = j_lo; dest_j < j_hi; ++dest_j) {
              for (int dest_k = k_lo; dest_k < k_hi; ++dest_k) {
                float weight = b_spline(pos, glm::vec3(dest_i, dest_j, dest_k), grid->h);
                grid->nodes[dest_i][dest_j][dest_k]->mass += weight * particle->mass;
                grid->nodes[dest_i][dest_j][dest_k]->velocity += weight * particle->mass * particle->velocity;
              }
            }
          }
        }
      }
    }
  }

  // Normalize grid velocity by its mass all at once
  for (int i = 0; i < grid->nodes.size(); ++i) {
    for (int j = 0; j < grid->nodes[i].size(); ++j) {
      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
        grid->nodes[i][j][k]->velocity /= grid->nodes[i][j][k]->mass;
      }
    }
  }
}

/*
* Step 2: compute the density and volume of each particle
*/
void compute_particle_volumes(Grid* grid) {
  float h3 = pow(grid->h, 3);
  for (int i = 0; i < grid->nodes.size(); ++i) {
    for (int j = 0; j < grid->nodes[i].size(); ++j) {
      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
        for (Particle* particle : grid->nodes[i][j][k]->particles) {
          glm::vec3 pos = particle->position;
          int i_lo = max((int) ceil(pos.x / grid->h - 2), 0);
          int i_hi = min((int) floor(pos.x / grid->h + 2) + 1, (int) grid->nodes.size());
          int j_lo = max((int) ceil(pos.y / grid->h - 2), 0);
          int j_hi = min((int) floor(pos.y / grid->h + 2) + 1, (int) grid->nodes[i].size());
          int k_lo = max((int) ceil(pos.z / grid->h - 2), 0);
          int k_hi = min((int) floor(pos.z / grid->h + 2) + 1, (int) grid->nodes[i][j].size());

          float density = 0;
          for (int dest_i = i_lo; dest_i < i_hi; ++dest_i) {
            for (int dest_j = j_lo; dest_j < j_hi; ++dest_j) {
              for (int dest_k = k_lo; dest_k < k_hi; ++dest_k) {
                float weight = b_spline(pos, glm::vec3(dest_i, dest_j, dest_k), grid->h);
                density += weight * grid->nodes[dest_i][dest_j][dest_k]->mass;
              }
            }
          }
          density /= h3;
          particle->volume = particle->mass / density;
        }
      }
    }
  }
}

/*
* preparation for Step 3: compute F_hat_Ep for each particle
* can maybe merge this with compute_grid_forces later
*   to save one full iteration over the grid cells and Particle* pointers?
*/
void compute_F_hat_Ep(Grid* grid, float delta_t) {
  float h3 = pow(grid->h, 3);
  for (int i = 0; i < grid->nodes.size(); ++i) {
    for (int j = 0; j < grid->nodes[i].size(); ++j) {
      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
        for (Particle* particle : grid->nodes[i][j][k]->particles) {
          glm::vec3 pos = particle->position;
          int i_lo = max((int) ceil(pos.x / grid->h - 2), 0);
          int i_hi = min((int) floor(pos.x / grid->h + 2) + 1, (int) grid->nodes.size());
          int j_lo = max((int) ceil(pos.y / grid->h - 2), 0);
          int j_hi = min((int) floor(pos.y / grid->h + 2) + 1, (int) grid->nodes[i].size());
          int k_lo = max((int) ceil(pos.z / grid->h - 2), 0);
          int k_hi = min((int) floor(pos.z / grid->h + 2) + 1, (int) grid->nodes[i][j].size());

          glm::mat3 sum = glm::mat3(0.0f);
          for (int dest_i = i_lo; dest_i < i_hi; ++dest_i) {
            for (int dest_j = j_lo; dest_j < j_hi; ++dest_j) {
              for (int dest_k = k_lo; dest_k < k_hi; ++dest_k) {
                glm::vec3 weight_grad = b_spline_grad(pos, glm::vec3(dest_i, dest_j, dest_k), grid->h);
                glm::vec3 velocity = grid->nodes[dest_i][dest_j][dest_k]->velocity;
                sum += delta_t * outerProduct(velocity, weight_grad);
              }
            }
          }
          glm::mat3 identity = glm::mat3(1.0f);
          particle->F_hat_Ep = (identity + sum) * particle->deformation_grad_P;
        }
      }
    }
  }
}

/*
* Step 3: compute force for each grid cell
*/
void compute_grid_forces(Grid* grid, float mu_0, float lambda_0, float xi) {
  float h3 = pow(grid->h, 3);
  for (int i = 0; i < grid->nodes.size(); ++i) {
    for (int j = 0; j < grid->nodes[i].size(); ++j) {
      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
        for (Particle* particle : grid->nodes[i][j][k]->particles) {
          glm::vec3 pos = particle->position;
          int i_lo = max((int) ceil(pos.x / grid->h - 2), 0);
          int i_hi = min((int) floor(pos.x / grid->h + 2) + 1, (int) grid->nodes.size());
          int j_lo = max((int) ceil(pos.y / grid->h - 2), 0);
          int j_hi = min((int) floor(pos.y / grid->h + 2) + 1, (int) grid->nodes[i].size());
          int k_lo = max((int) ceil(pos.z / grid->h - 2), 0);
          int k_hi = min((int) floor(pos.z / grid->h + 2) + 1, (int) grid->nodes[i][j].size());

          float volume = particle->volume;
          glm::mat3 sigma_p = psi_derivative(mu_0, lambda_0, xi, particle) * transpose(particle->deformation_grad_E);

          glm::mat3 neg_force_unweighted = volume * sigma_p;

          for (int dest_i = i_lo; dest_i < i_hi; ++dest_i) {
            for (int dest_j = j_lo; dest_j < j_hi; ++dest_j) {
              for (int dest_k = k_lo; dest_k < k_hi; ++dest_k) {
                glm::vec3 weight_grad = b_spline_grad(pos, glm::vec3(dest_i, dest_j, dest_k), grid->h);
                grid->nodes[dest_i][dest_j][dest_k]->force -= neg_force_unweighted * weight_grad;
              }
            }
          }
        }
      }
    }
  }
}

/*
* Step 7: update deformation gradients for each particle
*/
void update_deformation_gradients(Grid* grid, float theta_c, float theta_s, float delta_t) {
  for (int i = 0; i < grid->nodes.size(); ++i) {
    for (int j = 0; j < grid->nodes[i].size(); ++j) {
      for (int k = 0; k < grid->nodes[i][j].size(); ++k) {
        for (Particle* particle : grid->nodes[i][j][k]->particles) {
          glm::vec3 pos = particle->position;
          int i_lo = max((int) ceil(pos.x / grid->h - 2), 0);
          int i_hi = min((int) floor(pos.x / grid->h + 2) + 1, (int) grid->nodes.size());
          int j_lo = max((int) ceil(pos.y / grid->h - 2), 0);
          int j_hi = min((int) floor(pos.y / grid->h + 2) + 1, (int) grid->nodes[i].size());
          int k_lo = max((int) ceil(pos.z / grid->h - 2), 0);
          int k_hi = min((int) floor(pos.z / grid->h + 2) + 1, (int) grid->nodes[i][j].size());

          glm::mat3 grad_vp = glm::mat3(0.0f);

          for (int dest_i = i_lo; dest_i < i_hi; ++dest_i) {
            for (int dest_j = j_lo; dest_j < j_hi; ++dest_j) {
              for (int dest_k = k_lo; dest_k < k_hi; ++dest_k) {
                glm::vec3 weight_grad = b_spline_grad(pos, glm::vec3(dest_i, dest_j, dest_k), grid->h);
                glm::vec3 velocity = grid->nodes[dest_i][dest_j][dest_k]->velocity;
                grad_vp += outerProduct(velocity, weight_grad);
              }
            }
          }
          glm::mat3 identity = glm::mat3(1.0f);
          glm::mat3 dgrad_E_next = (identity + delta_t * grad_vp) * particle->deformation_grad_E;
          glm::mat3 F_next = dgrad_E_next * particle->deformation_grad_P;

          Matrix3f dgrad_E_eigen = glm_to_eigen(dgrad_E_next);
          JacobiSVD<MatrixXf> svd(dgrad_E_eigen, ComputeFullU | ComputeFullV);
          Matrix3f U_eigen = svd.matrixU();
          Matrix3f V_eigen = svd.matrixV();
          Vector3f S_eigen = svd.singularValues();
          glm::mat3 U = eigen_to_glm(U_eigen);
          glm::mat3 V = eigen_to_glm(V_eigen);
          glm::vec3 S_hat_vec = eigen_to_glm(S_eigen);

          glm::vec3 S_vec = glm::clamp(S_hat_vec, 1-theta_c, 1+theta_s);
          glm::mat3 S = glm::mat3(S_vec[0], 0.0, 0.0,
                                  0.0, S_vec[1], 0.0,
                                  0.0, 0.0, S_vec[2]);
          glm::mat3 S_inv = glm::mat3(1.0/S_vec[0], 0.0, 0.0,
                                  0.0, 1.0/S_vec[1], 0.0,
                                  0.0, 0.0, 1.0/S_vec[2]);

          particle->deformation_grad_E = U * S * transpose(V);
          particle->deformation_grad_P = V * S_inv * transpose(U) * F_next;
        }
      }
    }
  }
}

///////////////////////////////

void print_grid_node(GridNode* node) {
  cout << "GridNode(" << " ";
  cout << node->mass << " ";
  cout << glm::to_string(node->index) << " ";
  cout << glm::to_string(node->velocity) << " ";
  cout << ")\n";
}

void test_grid_node() {
  GridNode* node = new GridNode();
  print_grid_node(node);
}