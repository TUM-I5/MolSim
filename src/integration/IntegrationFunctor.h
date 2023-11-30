#pragma once

#include <memory>
#include <vector>

#include "particles/containers/ParticleContainer.h"

/**
 * @brief Interface for numerical integration methods
 *
 * This is used to ensure inheriting classes implement the method step, which performs one step with the respective integration method, thus
 * updating the positions, velocities and forces of the particles.
 */
class IntegrationFunctor {
   public:
    /**
     * @brief Virtual default destructor for the interface
     */
    virtual ~IntegrationFunctor() = default;

    /**
     * @brief Performs one step with the respective integration method.
     *
     * @param particle_container Container of particles on which the integration step is applied
     * @param force_sources Vector of force sources which are used to calculate the new forces
     * @param delta_t Time step
     */
    virtual void step(std::unique_ptr<ParticleContainer>& particle_container,
                      const std::vector<std::unique_ptr<ForceSource>>& force_sources, double delta_t) const = 0;
};