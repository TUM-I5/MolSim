#pragma once

#include "containers/ParticleContainer.h"

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
    virtual void spawnParticles(std::unique_ptr<ParticleContainer>& particle_container) const = 0;
};