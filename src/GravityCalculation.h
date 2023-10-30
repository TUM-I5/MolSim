#pragma once

#include "ForceSource.h"

/**
 * Implementation of the force calculation to simulate gravity forces between planets.
 * This class implements the interface ForceSource.
 */
class GravitationalForce : public ForceSource {
   public:
    /**
     * @brief Calculates the gravitational forces between two particles
     * @param p Particle
     * @param q Particle
     * @return Gravitational force exerted by q on p
     *
     * Calculates the gravitational force which q exerts on p
     */
    const std::array<double, 3UL> calculateForce(Particle& p, Particle& q) override;
};
