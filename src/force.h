#include <glm/glm.hpp>
#include <Eigen/SVD>

#include "particle.h"

using namespace std;
using namespace Eigen;

float lame_mu(float mu_0, float xi, float J_p);

float lame_lambda(float lambda_0, float xi, float J_p);

Matrix3f glm_to_eigen(glm::mat3 F);

glm::mat3 eigen_to_glm(Matrix3f F);

glm::vec3 eigen_to_glm(Vector3f v);

glm::mat3 polar_R(glm::mat3 F);

glm::mat3 psi_derivative(float mu_0, float lambda_0, float xi, Particle* particle);

void test_svd();

void test_glm_to_eigen();

void test_eigen_to_glm();