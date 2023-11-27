#pragma once

#include "particles/ParticleContainer.h"

/**
 * @brief Interface for particle spawner classes
 *
 * Definition of an interface for particle spawner classes. This is used to ensure inheriting classes implement the method
 * spawnParticles(ParticleContainer& particle_container) according to our definition.
 */
class ParticleSpawner {
   public:
    /**
     * @brief Spawns particles in the given container
     * @param particle_container Container to spawn particles in
     *
     * Spawns particles in the given container.
     */
    virtual void spawnParticles(ParticleContainer& particle_container) const = 0;

    /**
     * @brief Estimate the number of particles to be spawned
     *
     * returns the number of particles to be spawned by this spawner
     * this can be used to reserve enought memory in the particle container
     */
    virtual size_t getEstimatedNumberOfParticles() const = 0;
};