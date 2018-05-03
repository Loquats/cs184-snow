#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include <vector>
#include "particle.h"
#include "collision/collisionObject.h"
#include <cassert>

using namespace std;
using namespace glm;

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

  int grid_x, grid_y, grid_z;
  int dim_x, dim_y, dim_z;
  float h;
	Grid(int dim_x, int dim_y, int dim_z, float grid_h): h(grid_h) {

		bool divisible = fmod(dim_x, h) == 0 and fmod(dim_y, h) == 0 and fmod(dim_z, h) == 0;
		assert(divisible);
		this->dim_x = dim_x;
		this->dim_y = dim_y;
		this->dim_z = dim_z;
		this->grid_x = int(dim_x / h);
		this->grid_y = int(dim_y / h);
		this->grid_z = int(dim_z / h);
		first_step = true;
		nodes = vector<vector<vector<GridNode *> > >(grid_x);
		for (int i = 0; i < grid_x; ++i) {
			nodes[i] = vector<vector<GridNode *> >(grid_y);
			for (int j = 0; j < grid_y; ++j) {
				nodes[i][j] = vector<GridNode *>(grid_z);
				for (int k = 0; k < grid_z; ++k) {
					nodes[i][j][k] = new GridNode();
					nodes[i][j][k]->index = vec3(i, j, k);
				}
			}
		}
	};
    vector<vector<vector<GridNode *> > > nodes; // eventually should probably make this private
	vector<Particle *> all_particles;
	bool first_step;

	void resetGrid();
	void loadParticles(vector<Particle *> particles) { this->all_particles = particles; };
	void simulate(float delta_t, vector<vec3> external_accelerations, vector<CollisionObject *> *collision_objects, float E0);

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
