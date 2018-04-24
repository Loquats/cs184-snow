#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include <vector>
#include "particle.h"

using namespace std;

struct GridNode {
	glm::vec3 index;	// the {i, j, k} index of the grid node

	float mass;			// interpolated mass
	glm::vec3 velocity;	// interpolated velocity
	glm::vec3 force;
	vector<Particle *> particles;
};

// typedef vector<vector<vector<GridNode *> > > Grid;
struct Grid {

  vector<vector<vector<GridNode *> > > nodes;
  float h;
	Grid(int dim_x, int dim_y, int dim_z, float grid_h): h(grid_h) {
		nodes = vector<vector<vector<GridNode *> > >(dim_x);
		for (int i = 0; i < dim_x; ++i) {
			nodes[i] = vector<vector<GridNode *> >(dim_y);
			for (int j = 0; j < dim_y; ++j) {
				nodes[i][j] = vector<GridNode *>(dim_z);
				for (int k = 0; k < dim_z; ++k) {
					nodes[i][j][k] = new GridNode();
					nodes[i][j][k]->index = glm::vec3(i, j, k);
				}
			}
		}
	};

};

/*
* Step 1: compute the mass and velocity of each grid node based the particles
* in its neighborhood
*/
void particle_to_grid(Grid* grid);

/*
* Step 2: compute the density and volume of each particle
*/
void compute_particle_volumes(Grid* grid);

/*
* preparation for Step 3: compute F_hat_Ep for each particle
*/
void compute_F_hat_Ep(Grid* grid, float delta_t);

/*
* Step 3: compute force for each grid cell
*/
void compute_grid_forces(Grid* grid, float mu_0, float lambda_0, float xi);

#endif /* GRID_H */
