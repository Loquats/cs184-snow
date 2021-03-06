#include <algorithm>
#include <iostream>
#include "particle.h"
#include "grid.h"
#include "force.h"
#include <glm/glm.hpp>
#include <Eigen/SVD>
#include <glm/gtx/string_cast.hpp>

using namespace Eigen;
using namespace glm;

void Grid::particle_parallel_for(void (*f)(Particle*)) {
  std::vector< std::future<void> > results;
  int num_particles = int(all_particles.size() / num_threads);
  int counter = 0;

  while(counter < all_particles.size()) {
    std::future<void> result = thread_pool->enqueue([](void (*f)(Particle*), int start, int end, vector<Particle *> *all_particles)
      {
        for (int i = start; i < end && i < all_particles->size(); i++) {
          Particle *particle = (*all_particles)[i];
          f(particle);
        }
      }
      , f, counter, counter+num_particles, &all_particles);
    results.push_back(std::move(result));
    counter += num_particles;
  }
  for(auto && result: results) {
    result.wait();
    result.get();
  }
}


void Grid::resetGrid() {

    for (GridNode *node : nodes_in_use) {
        node->mass = 0;
        node->velocity = vec3(0.0);
        node->next_velocity = vec3(0.0);
        node->force = vec3(0.0);
        node->particles.clear();
    }
//    particle_parallel_for(reset_grid_worker);
    for (Particle *particle : all_particles) {
        // I'm not 100% sure rounding is the right thing to do, but the b-spline is centered on the grid index itself so I think it's right
        // Gonna use floor for now to be consistent with the rest of the code
        particle->position = glm::max(vec3(0.0), glm::min(particle->position, vec3(dim_x, dim_y, dim_z) - vec3(1e-5)));
        ivec3 index = glm::floor(particle->position / h);

        // We need to compute the new neighborhood bounds before we try instantiating needed GridNodes in the interpolation radius.
        particle->compute_neighborhood_bounds();
        particle->compute_b_spline_grad();
        for (int dest_i = particle->i_lo; dest_i < particle->i_hi; ++dest_i) {
            for (int dest_j = particle->j_lo; dest_j < particle->j_hi; ++dest_j) {
                for (int dest_k = particle->k_lo; dest_k < particle->k_hi; ++dest_k) {
                    GridNode *node = nodes[dest_i][dest_j][dest_k];
                    if (node == NULL) {
                        node = new GridNode();
                        node->index = ivec3(dest_i, dest_j, dest_k);
                        nodes[dest_i][dest_j][dest_k] = node;
                    }
                    nodes_in_use.insert(node);
                }
            }
        }
        // We need to instantiate the nodes that are to be in use, before we try pushing the particle
        GridNode *node = nodes[index.x][index.y][index.z];
        node->particles.push_back(particle);
    }

}

// Particles move over time and nodes fall in and out of use. We prune to avoid building up a bunch of unused grid nodes.
void Grid::pruneUnusedNodes() {

    cout << "(pruning nodes)";

    for (GridNode *node : nodes_in_use) {
        ivec3 index = node->index;
        nodes[index.x][index.y][index.z] = NULL;
        delete node;
    }

    nodes_in_use.clear();
//
//    for (Particle *particle : all_particles) {
//        ivec3 index = glm::floor(particle->position);
//        particle->compute_neighborhood_bounds(res_x, res_y, res_z, h);
//        for (int dest_i = particle->i_lo; dest_i < particle->i_hi; ++dest_i) { // iterate over everything in interpolation radius
//            for (int dest_j = particle->j_lo; dest_j < particle->j_hi; ++dest_j) {
//                for (int dest_k = particle->k_lo; dest_k < particle->k_hi; ++dest_k) {
//                    GridNode *node = nodes[dest_i][dest_j][dest_k];
//                    if (node != NULL) { // if NULL, we deal with it in resetGrid
//                        nodes_in_use.insert(node); // pointers naturally are comparable for a set
//                    }
//                }
//            }
//        }
//    }
//
//    int new_size = nodes_in_use.size();
//
//    cout << "nodes before pruning: " << old_size << endl;
//    cout << "nodes after pruning: " << new_size << endl;
}

void Grid::simulate(float delta_t, vector<vec3> external_accelerations, vector<CollisionObject *> *collision_objects, PhysicsParams* params) {

    // Every so often, prune unused nodes so that we don't run all the computations on them.
    // We set steps_since_node_reset so that the if condition fires when {reset_time} seconds have elapsed.
    ++steps_since_node_reset;
    if (steps_since_node_reset > reset_time / delta_t) {
        pruneUnusedNodes();
        steps_since_node_reset = 0;
    }

    resetGrid();
    particle_to_grid();                       // Step 1.
    if (first_step) {
        compute_particle_volumes();           // Step 2.
        first_step = false;
    }
    compute_F_hat_Ep(delta_t);                // (Step 3.)
    compute_grid_forces(params->mu_0, params->lambda_0, params->xi);  // Step 3.
//    apply_ext_accelerations(external_accelerations);     // not a step
    compute_grid_velocities(delta_t, collision_objects);    // Step 4.
                                                            // Step 5: grid-base collisions (skipped)
    // compute_time_integration();                          // Step 6: disabled b/c do not use semi-implicit integratino
    update_deformation_gradients(params->theta_c, params->theta_s, delta_t);// Step 7.
    update_particle_velocities(params->alpha);                      // Step 8.

    // the code below could get wrapped up in a method, i just didn't bother since it's sorta in test still
    // also kinda unsure of whether we're supposed to apply accelerations on the grid or the particles

    // External accelerations (eg. gravity) applied on all particles
    vec3 total_acc = vec3(0.0);
    for (vec3 acc : external_accelerations) {
      total_acc += acc;
    }
    for (Particle *particle : all_particles) {
      particle->velocity += total_acc * delta_t;
    }

    // Step 9.
    compute_particle_collisions(delta_t, collision_objects);

    // Step 10.
    update_particle_positions(delta_t);
}

/*
* Step 1: compute the mass and velocity of each grid node based the particles
* in its neighborhood
*/
void Grid::particle_to_grid() {
  for (Particle *particle : all_particles) {
    vec3 pos = particle->position;
    for (int dest_i = particle->i_lo; dest_i < particle->i_hi; ++dest_i) {
      for (int dest_j = particle->j_lo; dest_j < particle->j_hi; ++dest_j) {
        for (int dest_k = particle->k_lo; dest_k < particle->k_hi; ++dest_k) {
          float weight = particle->b_spline_at(dest_i, dest_j, dest_k);
          nodes[dest_i][dest_j][dest_k]->mass += weight * particle->mass;
          nodes[dest_i][dest_j][dest_k]->velocity += weight * particle->mass * particle->velocity;
        }
      }
    }
  }

  // Normalize grid velocity by its mass all at once
  for (GridNode *node : nodes_in_use) {
      if (node->mass > 0) {
          node->velocity /= node->mass;
      }
  }
}

/*
* Step 2: compute the density and volume of each particle
*/
void Grid::compute_particle_volumes() {
  float h3 = pow(h, 3);
  for (Particle *particle : all_particles) {
    float density = 0;
    for (int dest_i = particle->i_lo; dest_i < particle->i_hi; ++dest_i) {
      for (int dest_j = particle->j_lo; dest_j < particle->j_hi; ++dest_j) {
        for (int dest_k = particle->k_lo; dest_k < particle->k_hi; ++dest_k) {
          float weight = particle->b_spline_at(dest_i, dest_j, dest_k);
          density += weight * nodes[dest_i][dest_j][dest_k]->mass;
        }
      }
    }
    density /= h3;
    particle->volume = particle->mass / density;
    particle->cbrt_volume = cbrt(particle->volume);
  }
}

/*
* preparation for Step 3: compute F_hat_Ep for each particle
* can maybe merge this with compute_grid_forces later
*   to save one full iteration over the grid cells and Particle* pointers?
*/
void Grid::compute_F_hat_Ep(float delta_t) {
  float h3 = pow(h, 3);
  for (Particle *particle : all_particles) {
    mat3 sum = mat3(0.0f);
    for (int dest_i = particle->i_lo; dest_i < particle->i_hi; ++dest_i) {
      for (int dest_j = particle->j_lo; dest_j < particle->j_hi; ++dest_j) {
        for (int dest_k = particle->k_lo; dest_k < particle->k_hi; ++dest_k) {
          vec3 weight_grad = particle->b_spline_grad_at(dest_i, dest_j, dest_k);
          vec3 velocity = nodes[dest_i][dest_j][dest_k]->velocity;
          sum += delta_t * outerProduct(velocity, weight_grad);
        }
      }
    }
    mat3 identity = mat3(1.0f);
    particle->F_hat_Ep = (identity + sum) * particle->deformation_grad_E;
  }
}

/*
* Step 3: compute force for each grid cell
*/
void Grid::compute_grid_forces(float mu_0, float lambda_0, float xi) {
  float h3 = pow(h, 3);
  for (Particle *particle : all_particles) {
    float volume = particle->volume;
    mat3 sigma_p = psi_derivative(mu_0, lambda_0, xi, particle) * transpose(particle->deformation_grad_E);

    mat3 neg_force_unweighted = volume * sigma_p;

    for (int dest_i = particle->i_lo; dest_i < particle->i_hi; ++dest_i) {
      for (int dest_j = particle->j_lo; dest_j < particle->j_hi; ++dest_j) {
        for (int dest_k = particle->k_lo; dest_k < particle->k_hi; ++dest_k) {
          vec3 weight_grad = particle->b_spline_grad_at(dest_i, dest_j, dest_k);
          nodes[dest_i][dest_j][dest_k]->force -= neg_force_unweighted * weight_grad;
        }
      }
    }
  }
}

/*
 * Apply external forces, e.g. gravity
 */
void Grid::apply_ext_accelerations(vector<vec3> external_accelerations) {
    for (GridNode *node : nodes_in_use) {
        for (vec3 acc : external_accelerations) {
            node->force += acc * node->mass;
        }
    }
}

/*
 * Step 4 and 5: update grid node velocities, do collisions
 */
void Grid::compute_grid_velocities(float delta_t, vector<CollisionObject *> *collision_objects) {
    for (GridNode *node : nodes_in_use) {
        node->next_velocity = node->velocity;
        if (node->mass > 0) {
            node->next_velocity += node->force * delta_t / node->mass;
        }
        vec3 position = node->index * h;
        for (CollisionObject* co : *collision_objects) {
             node->next_velocity = co->collide(position, node->next_velocity, delta_t);
        }
    }
}

/*
 * Step 6: time integration
 */
void Grid::compute_time_integration() {
  return;
  for (int i = 0; i < res_x; ++i) {
    for (int j = 0; j < res_y; ++j) {
      for (int k = 0; k < res_z; ++k) {
        // Explicit update: no need to do anything. next_velocity and velocity are both used in the next part
//        GridNode* node = nodes[i][j][k];
//        node->velocity = node->next_velocity;
      }
    }
  }
}

/*
* Step 7: update deformation gradients for each particle
*/
void Grid::update_deformation_gradients(float theta_c, float theta_s, float delta_t) {
  for (Particle *particle : all_particles) {
    mat3 grad_vp = mat3(0.0f);

    for (int dest_i = particle->i_lo; dest_i < particle->i_hi; ++dest_i) {
      for (int dest_j = particle->j_lo; dest_j < particle->j_hi; ++dest_j) {
        for (int dest_k = particle->k_lo; dest_k < particle->k_hi; ++dest_k) {
          vec3 weight_grad = particle->b_spline_grad_at(dest_i, dest_j, dest_k);
          vec3 velocity = nodes[dest_i][dest_j][dest_k]->next_velocity;
          grad_vp += outerProduct(velocity, weight_grad);
        }
      }
    }
    mat3 identity = mat3(1.0f);
    mat3 dgrad_E_next = (identity + delta_t * grad_vp) * particle->deformation_grad_E;
    mat3 F_next = dgrad_E_next * particle->deformation_grad_P;

    Matrix3f dgrad_E_eigen = glm_to_eigen(dgrad_E_next);
    JacobiSVD<MatrixXf> svd(dgrad_E_eigen, ComputeFullU | ComputeFullV);
    Matrix3f U_eigen = svd.matrixU();
    Matrix3f V_eigen = svd.matrixV();
    Vector3f S_eigen = svd.singularValues();
    mat3 U = eigen_to_glm(U_eigen);
    mat3 V = eigen_to_glm(V_eigen);
    vec3 S_hat_vec = eigen_to_glm(S_eigen);

    vec3 S_vec = clamp(S_hat_vec, 1-theta_c, 1+theta_s);
    mat3 S = mat3(S_vec[0], 0.0, 0.0,
                            0.0, S_vec[1], 0.0,
                            0.0, 0.0, S_vec[2]);
    mat3 S_inv = mat3(1.0/S_vec[0], 0.0, 0.0,
                            0.0, 1.0/S_vec[1], 0.0,
                            0.0, 0.0, 1.0/S_vec[2]);

    particle->deformation_grad_E = U * S * transpose(V);
    particle->deformation_grad_P = V * S_inv * transpose(U) * F_next;
  }
}

/*
 * Step 8: Update particle velocities using PIC and FLIP parts
 */
void Grid::update_particle_velocities(float alpha) {
  for (Particle *particle : all_particles) {
    vec3 v_pic = vec3(0.0);
    vec3 v_flip = particle->velocity;

    for (int dest_i = particle->i_lo; dest_i < particle->i_hi; ++dest_i) {
      for (int dest_j = particle->j_lo; dest_j < particle->j_hi; ++dest_j) {
        for (int dest_k = particle->k_lo; dest_k < particle->k_hi; ++dest_k) {
          GridNode *dest = nodes[dest_i][dest_j][dest_k];
          float weight = particle->b_spline_at(dest_i, dest_j, dest_k);
          v_pic += dest->next_velocity * weight;
          v_flip += (dest->next_velocity - dest->velocity) * weight;
        }
      }
    }
    particle->velocity = (1.f - alpha) * v_pic + alpha * v_flip;
  }
}

/*
 * Step 9: Particle-based collisions
 */
void Grid::compute_particle_collisions(float delta_t, vector<CollisionObject *> *objects) {
  for (Particle *particle : all_particles) {
    for (CollisionObject *co : *objects) {
//      vec3 next_position = particle->position + delta_t * particle->velocity;
      particle->velocity = co->collide(particle->position, particle->velocity, delta_t);
    }
  }
}

/*
 * Step 10: Update particle positions
 */
void Grid::update_particle_positions(float delta_t) {
  for (Particle *particle : all_particles) {
    particle->position += particle->velocity * delta_t;
  }
}

void Grid::reset_grid_worker(Particle *particle) {
  particle->compute_neighborhood_bounds();
  particle->compute_b_spline_grad();
}


///////////////////////////////

void print_grid_node(GridNode* node) {
  cout << "GridNode(" << " ";
  cout << node->mass << " ";
  cout << to_string(node->index) << " ";
  cout << to_string(node->velocity) << " ";
  cout << ")\n";
}

void test_grid_node() {
  GridNode* node = new GridNode();
  print_grid_node(node);
}