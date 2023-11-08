#pragma once

#include <vector>

#include "physics/ForceSource.h"
#include "types/Particle.h"

/**
 * @brief Wrapper class for a set of particles
 *
 * Container class for particles, which provides some convenience methods.
 */
class ParticleContainer {
    std::vector<Particle> particles;

   public:
    /**
     * @brief Default constructor
     *
     * Generates an empty ParticleContainer object.
     * If the amount of particles is known, it is recommended to use the constructor with the amount of particles as
     * parameter.
     */
    ParticleContainer() = default;

    /**
     * @brief Constructor with amount of particles
     * @param n Amount of particles
     *
     * Generates an empty ParticleContainer object and reserves memory for the given amount of particles.
     */
    ParticleContainer(int n);

    /**
     * @brief Reserves memory for particles
     * @param n Amount of particles
     *
     * Reserves memory for the given amount of particles.
     */
    void reserve(int n);

    /**
     * @brief Returns the amount of particles
     * @return Amount of particles
     *
     * Returns the amount of particles in the container.
     */
    std::size_t size() const;

    /**
     * @brief Returns the capacity of the container
     * @return Capacity of the container
     *
     * Returns the capacity of the container.
     */
    std::size_t capacity() const;

    /**
     * @brief Adds a particle to the container
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    void addParticle(const Particle& p);

    /**
     * @brief Adds a particle to the container
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    void addParticle(Particle&& p);

    /**
     * @brief Returns a particle
     * @param i Index of the particle
     * @return Particle
     *
     * Returns the particle at the given index.
     */
    Particle& operator[](int i);

    /**
     * @brief Returns an iterator to the first particle
     * @return Iterator to the first particle
     *
     * Returns the begin iterator for the internal data structure.
     */
    std::vector<Particle>::iterator begin();

    /**
     * @brief Returns an end iterator for this container
     * @return End iterator for this container
     *
     * Returns the end iterator for the internal data structure.
     */
    std::vector<Particle>::iterator end();

    /**
     * @brief Returns a const iterator to the first particle
     * @return Const iterator to the first particle
     *
     * Returns the begin const iterator for the internal data structure.
     */
    std::vector<Particle>::const_iterator begin() const;

    /**
     * @brief Returns a const end iterator for this container
     * @return Const end iterator for this container
     *
     * Returns the end const iterator for the internal data structure.
     */
    std::vector<Particle>::const_iterator end() const;
};
