//
// Created by Layla Zadina on 09.11.2023.
//

#include "Formulaes.h"
#include "utils/ArrayUtils.h"
#include <list>
#include "ParticleContainer.h"

//TODO: refactoring of the MolSim class to extract the formulaes?

double sigma = 1;
double eps = 5;

/**
   * L2 norm (xi,xj)
   */

//template <double, std::size_t N>
double static secondNorm(const std::array<double, 3>& arr) {
    double norm = 0.0;
    for (const double& val : arr) {
        norm += val * val;
    }
    return std::sqrt(norm);
}

/**
   * Lennard-Jones potential
   */

double calculatePotential(std::array<double, 3> &xi, std::array<double, 3> &xj){
    //U(xi, xj) = 4*eps((sigma/L2_norm(xi,xj))^12 - (sigma/L2_norm(xi,xj))^6)
    auto potential = 4 * eps * (pow((sigma/ secondNorm(xi - xj)), 12.0) - pow((sigma/ secondNorm(xi - xj)), 6.0) );
    return potential;
}

/**
   * Lennard-Jones force
   */

static auto calculateLJForce(std::array<double, 3> &xi, std::array<double, 3> &xj){
    //
    auto force = (-24 * eps / pow(secondNorm(xi-xj), 2.0) ) *
            (pow((sigma/ secondNorm(xi - xj)), 6.0) - 2*pow((sigma/ secondNorm(xi - xj)), 12.0)) * (xi-xj);
    return force;


}
