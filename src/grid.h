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

void particle_to_grid(Grid* grid);
void compute_particle_volumes(Grid* grid);
#endif /* GRID_H */