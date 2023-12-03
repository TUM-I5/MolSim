#pragma once

#include "integration/IntegrationFunctor.h"

/**
 * @brief Implementation of the Strömer-Verlet integration method. Implements the interface IntegrationFunctor.
 *
 * Implements the IntegrationFunctor interface, and therefore updates all particles in the particle_container according to the Verlet
 * integration method.
 */
class VerletFunctor : public IntegrationFunctor {
   public:
    /**
     * @brief Performs one step with the respective integration method.
     *
     * @param particle_container Container of particles on which the integration step is applied
     * @param force_sources Vector of force sources which are used to calculate the new forces
     * @param delta_t Time step
     */
    void step(std::unique_ptr<ParticleContainer>& particle_container, const std::vector<std::unique_ptr<ForceSource>>& force_sources,
              double delta_t) const override;
};