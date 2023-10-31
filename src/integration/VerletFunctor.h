#pragma once

#include "integration/IntegrationFunctor.h"

/**
 * @brief Implementation of the Verlet integration method. Implements the interface IntegrationFunctor.
 *
 * Implements the IntegrationFunctor interface, and therefore updates all particles in the particle_container according to the Verlet
 * integration method.
 */
class VerletFunctor : public IntegrationFunctor {
   public:
    void step(ParticleContainer& particle_container, ForceSource& totalForce, double delta_t) override;
};