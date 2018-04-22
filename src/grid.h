#include <glm/glm.hpp>

using namespace std;

struct GridNode {
	glm::vec3 index;	// the {i, j, k} index of the grid node

	float mass;			// interpolated mass
	glm::vec3 velocity;	// interpolated velocity
	vector<Particle *> particles;
};

// typedef vector<vector<vector<GridNode *> > > Grid;
struct Grid {
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

	vector<vector<vector<GridNode *> > > nodes;
	float h;
};

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