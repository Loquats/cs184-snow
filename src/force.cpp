#include <iostream>
#include <glm/glm.hpp>
#include <Eigen/SVD>

#include "glm/ext.hpp"
#include "force.h"

using namespace std;
using namespace Eigen;

float lame_mu(float mu_0, float xi, float J_p) {
	return mu_0 * exp(xi * (1 - J_p));
}

float lame_lambda(float lambda_0, float xi, float J_p) {
	return lambda_0 * exp(xi * (1 - J_p));
}

Matrix3f glm_to_eigen(glm::mat3 F) {
	Matrix3f F_eigen;
	F_eigen << F[0][0], F[1][0], F[2][0], F[0][1], F[1][1], F[2][1], F[0][2], F[1][2], F[2][2];
	return F_eigen;
}

glm::mat3 eigen_to_glm(Matrix3f F) {
	glm::mat3 F_glm = glm::mat3(F(0, 0), F(1, 0), F(2, 0), F(0, 1), F(1, 1), F(2, 1), F(0, 2), F(1, 2), F(2, 2));
	return F_glm;
}

glm::vec3 eigen_to_glm(Vector3f v) {
	glm::vec3 v_glm = glm::vec3(v(0), v(1), v(2));
	return v_glm;
}

/*
Let the polar decomposition of F be F = RS. Return R.
Computed by taking the SVD of F as F = USV^t, and R = UV^*
`^*` denotes conjugate transpose (adjoint).
*/
glm::mat3 polar_R(glm::mat3 F) {
	Matrix3f F_eigen = glm_to_eigen(F);
	JacobiSVD<MatrixXf> svd(F_eigen, ComputeFullU | ComputeFullV);
	Matrix3f U = svd.matrixU();
	Matrix3f V = svd.matrixV();
	Matrix3f R = U * V.adjoint();
	return eigen_to_glm(R);
}

glm::mat3 psi_derivative(float mu_0, float lambda_0, float xi, Particle* particle) {
	float J_p = determinant(particle->deformation_grad_P);
	float J_e = determinant(particle->F_hat_Ep);
	glm::mat3 R_E = polar_R(particle->F_hat_Ep);
	return 2 * lame_mu(mu_0, xi, J_p) * (particle->F_hat_Ep - R_E) + lame_lambda(lambda_0, xi, J_p) * (J_e - 1) * J_e * inverse(transpose(particle->F_hat_Ep));
}

void test_svd() {
	MatrixXf m = MatrixXf::Random(3,2);
	cout << "Here is the matrix m:" << endl << m << endl;
	JacobiSVD<MatrixXf> svd(m, ComputeThinU | ComputeThinV);
	cout << "Its singular values are:" << endl << svd.singularValues() << endl;
	cout << "Its left singular vectors are the columns of the thin U matrix:" << endl << svd.matrixU() << endl;
	cout << "Its right singular vectors are the columns of the thin V matrix:" << endl << svd.matrixV() << endl;
	Vector3f rhs(1, 0, 0);
	cout << "Now consider this rhs vector:" << endl << rhs << endl;
	cout << "A least-squares solution of m*x = rhs is:" << endl << svd.solve(rhs) << endl;
}

void test_glm_to_eigen() {
	glm::mat3 m = glm::mat3(1, 2, 3, 4, 5, 6, 7, 8, 9);
	cout << glm::to_string(m) << endl;
	cout << glm_to_eigen(m) << endl;
}

void test_eigen_to_glm() {
	Matrix3f m;
	m << 1, 2, 3, 4, 5, 6, 7, 8, 9;
	cout << m << endl;
	cout << glm::to_string(eigen_to_glm(m)) << endl;
}