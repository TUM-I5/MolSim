#pragma once

#include "physics/ForceSource.h"

/**
 *  @brief Class to calculate gravitational forces between particles. Implements the interface ForceSource.
 *
 * Implementation of the force calculation to simulate gravity forces between particles.
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
    std::array<double, 3UL> calculateForce(Particle& p, Particle& q) const override;

    /**
     * @brief Returns the name of the force source
     */
    operator std::string() const override;
};
