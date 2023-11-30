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
    const double epsilon;

    /**
     * @brief Lennard-Jones potential parameter sigma
     */
    const double sigma;

   public:
    explicit LennardJonesForce(double epsilon = 5.0, double sigma = 1.0) : epsilon(epsilon), sigma(sigma) {}

    /**
     * @brief Calculates the Lennard-Jones forces between two particles
     *
     * @param p Particle
     * @param q Particle
     * @return Lennard-Jones force exerted by q on p
     *
     * Calculates the Lennard-Jones force which q exerts on p
     */
    std::array<double, 3UL> calculateForce(Particle& p, Particle& q) const override;

    /**
     * @brief Returns "Lennard-Jones" as name of the force
     */
    explicit operator std::string() const override;
};
