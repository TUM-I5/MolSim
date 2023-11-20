#pragma once

#include <memory>
#include <vector>

#include "particles/Particle.h"
#include "physics/ForceSource.h"

class ParticleContainerInterface {
   public:
    /**
     * @brief Adds a particle to the container
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    virtual void addParticle(const Particle& p) = 0;

    /**
     * @brief Adds a particle to the container
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    virtual void addParticle(Particle&& p) = 0;

    /**
     * @brief Applies the given force sources to the particles
     * @param force_sources List of force sources to be applied
     *
     * Applies the given force sources to the particles in the container.
     * Uses newton's third law to calculate the forces between the particles in an optimized way.
     */
    virtual void applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) = 0;
};