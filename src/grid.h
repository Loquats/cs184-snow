#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include <vector>
#include <thread>
#include "particle.h"
#include "collision/collisionObject.h"
#include "misc/threadpool.h"
#include "misc/physics_params.h"
#include <set>

using namespace std;
using namespace glm;

const float reset_time = 0.1; // seconds of simulation to reset grid nodes

struct GridNode {
	vec3 index;	// the {i, j, k} index of the grid node

	float mass;			// interpolated mass
	vec3 velocity;		// interpolated velocity
	vec3 next_velocity; // for part 4, 5, 6
	vec3 force;
	vector<Particle *> particles;
};

class Grid {
public:

  float dim_x, dim_y, dim_z;
  int res_x, res_y, res_z;
  float h;
  ThreadPool* thread_pool;
	Grid(int res_x, int res_y, int res_z, float grid_h): h(grid_h) {

        num_threads = std::thread::hardware_concurrency();
            thread_pool = new ThreadPool(num_threads);
        this->res_x = res_x;
        this->res_y = res_y;
        this->res_z = res_z;
        dim_x = res_x * h;
        dim_y = res_y * h;
        dim_z = res_z * h;
        first_step = true;
        nodes = vector<vector<vector<GridNode *> > >(res_x);
        for (int i = 0; i < res_x; ++i) {
            nodes[i] = vector<vector<GridNode *> >(res_y);
            for (int j = 0; j < res_y; ++j) {
                nodes[i][j] = vector<GridNode *>(res_z);
            }
        }
	};
    vector<vector<vector<GridNode *> > > nodes; // eventually should probably make this private
	vector<Particle *> all_particles;
	int num_threads;
	void particle_parallel_for(void (*f)(Particle*));
	void gridnode_parallel_for(void (*f)(GridNode*));
	static void reset_grid_worker(Particle * particle);
	set<GridNode *> nodes_in_use;
	bool first_step = true;
	int steps_since_node_reset = 0;

    void pruneUnusedNodes();
	void resetGrid();
	void loadParticles(vector<Particle *> particles) { this->all_particles = particles; };
	void simulate(float delta_t, vector<vec3> external_accelerations, vector<CollisionObject *> *collision_objects, PhysicsParams* params);

	/*
	* Step 1: compute the mass and velocity of each grid node based the particles
	* in its neighborhood
	*/
	void particle_to_grid();

	/*
	* Step 2: compute the density and volume of each particle
	*/
	void compute_particle_volumes();

	/*
	* preparation for Step 3: compute F_hat_Ep for each particle
	*/
	void compute_F_hat_Ep(float delta_t);

	/*
	* Step 3: compute force for each grid cell
	*/
	void compute_grid_forces(float mu_0, float lambda_0, float xi);

	/*
	 * Apply external forces, namely gravity
	 */
	void apply_ext_accelerations(vector<vec3> external_accelerations);

	/*
	 * Step 4 and 5: update grid node velocities, collisions
	 */
	void compute_grid_velocities(float delta_t, vector<CollisionObject *> *collision_objects);

	/*
	 * Step 6: update velocity using either explicit or implicit time integration
	 */
	void compute_time_integration();

	/*
	* Step 7: update deformation gradients for each particle
	*/
	void update_deformation_gradients(float theta_c, float theta_s, float delta_t);

	/*
	 * Step 8: Update particle velocities using PIC and FLIP parts
	 */
	void update_particle_velocities(float alpha);

	/*
	 * Step 9: Particle-based collisions
	 */
	void compute_particle_collisions(float delta_t, vector<CollisionObject *> *objects);

	/*
	 * Step 10: Update particle positions
	 */
	void update_particle_positions(float delta_t);
};

#endif /* GRID_H */
