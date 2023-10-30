#pragma once

#include "Particle.h"

/**
 * Definition of an interface for force source classes.
 * This is used to ensure inheriting classes implement the method calculateForce(ParticleContainer& particles)
 * according to our definition.
 */
class ForceSource {
   public:
    /**
     * @brief Calculates the force a particle q exerts on another particle p
     * @param p Particle whose force is to be updated
     * @param q Particle which exerts the force on p
     * @return Force exerted by q on p
     *
     * Calculates the force a particle q exerts on another particle p.
     */
    virtual const std::array<double, 3UL> calculateForce(Particle& p, Particle& q) = 0;
};
