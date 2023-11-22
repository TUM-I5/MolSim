//
// Created by Layla Zadina on 09.11.2023.
//

#ifndef PSEMOLDYN_GROUPE_FORMULAS_H
#define PSEMOLDYN_GROUPE_FORMULAS_H

#include <array>
#include "ParticleContainer.h"
#include <utility> // std::pair, std::make_pair


class Formulas {

public:

    /**
   * L2 norm of (xi, xj)
   */
    static double secondNorm(const std::array<double, 3> &arr1);

    /**
   * Lennard-Jones potential
   */
    double calculatePotential(std::array<double, 3> &xi, std::array<double, 3> &xj, double sigma, double eps);

    /**
  * Lennard-Jones force
  */
    static void calculateLJForce(std::pair<Particle, Particle> ppair, double sigma, double eps);

    /**
 * calculate the force for all particles
 */
    static void calculateF(ParticleContainer pc);

/**
 * calculate the position for all particles
 */
    static void calculateX(double delta_t, ParticleContainer pc);

/**
 * calculate the position for all particles
 */
    static void calculateV(double delta_t, ParticleContainer pc);

/**
 * plot the particles to a xyz-file
 */
    static void plotParticles(int iteration, ParticleContainer pc);
};

#endif //PSEMOLDYN_GROUPE_FORMULAS_H
