#pragma once

#include "Particle.h"

/*
 * Definition of an interface for force calculation classes.
 * This is used to ensure inheriting classes implement the method addForce(ParticleContainer& particles)
 * according to our definition.
 */
class ForceCalculation {
   public:
    /*
     * @brief Adds the force exerted by one particle to the others force value
     * @param p Particle whose force is to be updated
	 * @param q Particle which exerts the force on p
     *
     * Calculates the force a particle q exerts on another particle p and adds it to p's force value.
     */
    virtual void addForce(Particle& p, Particle& q) = 0;
};
