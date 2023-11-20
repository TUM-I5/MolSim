#pragma once

#include <memory>
#include <vector>

#include "particles/Particle.h"
#include "physics/ForceSource.h"

class ParticleContainer {
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

    /**
     * @brief Reserves space for n particles
     *
     * @param n Amount of particles to store in the container
     */
    virtual void reserve(size_t n) = 0;

    /**
     * @brief Returns the number of particles in the container
     *
     * @return Number of particles in the container
     */
    virtual size_t size() const = 0;

    /**
     * @brief Overload of the [] operator to access the particles in the container
     *
     * @param i
     * @return Particle&
     */
    virtual Particle& operator[](int i) = 0;

    /**
     * @brief The begin iterator for the internal data structure.
     *
     * @return Iterator to the first particle
     */
    virtual std::vector<Particle>::iterator begin() = 0;

    /**
     * @brief The end iterator for the internal data structure.
     *
     * @return Iterator to the end of the container
     */
    virtual std::vector<Particle>::iterator end() = 0;

    /**
     * @brief The begin const iterator for the internal data structure.
     *
     * @return Const iterator to the first particle
     */
    virtual std::vector<Particle>::const_iterator begin() const = 0;

    /**
     * @brief The end const iterator for the internal data structure.
     *
     * @return Const iterator to the end of the container
     */
    virtual std::vector<Particle>::const_iterator end() const = 0;
};