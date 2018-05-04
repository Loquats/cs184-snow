#ifndef PHYSICS_PARAMS_H
#define PHYSICS_PARAMS_H

struct PhysicsParams {
  float E_0;            // Young's modulus
  float nu;             // Poisson's ratio
  float xi;             // Hardening coefficient
  float theta_c;        // Critical compression
  float theta_s;        // Critical stretch
  float alpha;          // FLIP/PIC ratio
  float mu_0;           // Lame parameter
  float lambda_0;       // Lame paramter

  PhysicsParams(float E_0, float nu, float xi, float theta_c, float theta_s, float alpha): 
    E_0(E_0), nu(nu), xi(xi), theta_c(theta_c), theta_s(theta_s), alpha(alpha) {

    mu_0 = E_0 / (2. * (1. + nu));
    lambda_0 = E_0 * nu / ((1. + nu) * (1. - 2. * nu));
  }
};

#endif // PHYSICS_PARAMS_H