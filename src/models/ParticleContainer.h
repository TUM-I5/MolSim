//
// Created by Alp Kaan Aksu on 29.10.23.
//

#pragma once

#include <vector>
#include <functional>

#include "models/Particle.h"

/**
 * A data structure that stores all particles in simulations
 * and provides a convenient way to iterate over (pairs of) them
 */
class ParticleContainer {
private:
    /**
     * The vector that contains all the particles in the container
     */
    std::vector<Particle> particles;
public:
    ParticleContainer();

    /**
     * Apply a function to all particles (one by one)
     *
     * @param function
     */
    void applyToAll(const std::function<void(Particle &)>& function);

    /**
     * Apply a function to all pairs of particles
     *
     * @param function
     */
    void applyToAllPairs(const std::function<void(Particle &, Particle &)> &function);

    /**
     * Add a new particle to the container
     *
     * @param particle
     */
    void add(const Particle &particle);

    /**
     * Return the number of particles in the container
     *
     * @return Number of particles
     */
    int size();
};