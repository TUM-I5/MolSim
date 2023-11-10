#pragma once

#include "physics/ForceSource.h"

/**
 *  @brief Class to calculate the Lennard-Jones force between particles. Implements the interface ForceSource.
 *
 * Implementation of the force calculation to simulate Lennard-Jones forces between particles.
 */
class LennardJonesForce : public ForceSource {
   public:
    /**
     * @brief Lennard-Jones potential parameter epsilon
     */
    double epsilon;

    /**
     * @brief Lennard-Jones potential parameter sigma
     */
    double sigma;

   public:
    LennardJonesForce(double epsilon = 5.0, double sigma = 1.0) : epsilon(epsilon), sigma(sigma) {}

    /**
     * @brief Calculates the Lennard-Jones forces between two particles
     * @param p Particle
     * @param q Particle
     * @return Lennard-Jones force exerted by q on p
     *
     * Calculates the Lennard-Jones force which q exerts on p
     */
    std::array<double, 3UL> calculateForce(Particle& p, Particle& q) override;

    /**
     * @brief Returns the name of the force source
     * @return Name of the force source
     *
     * Returns the name of the force source.
     */
    std::string getName() override;
};
