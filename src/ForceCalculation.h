#pragma once

#include "ParticleContainer.h"

/*
 * Definition of an interface for force calculation classes.
 * This is used to ensure inheriting classes implement the method calculateForces(ParticleContainer& particles)
 * according to our definition.
 */
class ForceCalculation {
   public:
    /*
     * @brief Calculates the forces between the particles in the given container
     * @param particles Container of particles
     *
     * Calculates the forces between all particles in the given container and updates their values.
     * The current force is saved in the particle's f attribute, while the value previous to the update will be saved in
     * the particle's oldF attribute. This method must be implemented by every inheriting class.
     */
    virtual void calculateForces(ParticleContainer& particles) = 0;
};
