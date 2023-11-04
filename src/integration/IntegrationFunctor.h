#pragma once

#include <memory>
#include <vector>

#include "types/ParticleContainer.h"

/**
 * @brief Interface for numerical integration methods
 *
 * This is used to ensure inheriting classes implement the method step, which performs one step with the respective integration method, thus
 * updating the positions, velocities and forces of the particles.
 */
class IntegrationFunctor {
   public:
    /**
     * @brief Performs one step with the respective integration method.
     *
     * @param particle_container
     * @param totalForce
     * @param delta_t
     */
    virtual void step(ParticleContainer& particle_container, std::vector<std::unique_ptr<ForceSource>>& force_sources, double delta_t) = 0;
};