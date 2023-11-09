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
     * @brief Calculates the Lennard-Jones forces between two particles
     * @param p Particle
     * @param q Particle
     * @return Lennard-Jones force exerted by q on p
     *
     * Calculates the Lennard-Jones force which q exerts on p
     */
    std::array<double, 3UL> calculateForce(Particle& p, Particle& q) override;
};
