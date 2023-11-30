#pragma once

#include <memory>
#include <vector>

#include "particles/Particle.h"
#include "particles/containers/ParticleContainer.h"
#include "physics/ForceSource.h"

/**
 * @brief Wrapper class for a set of particles
 *
 * Container class for particles, which provides some convenience methods.
 */
class DirectSumContainer : public ParticleContainer {
    /**
     * @brief Internal data structure for the particles
     */
    std::vector<Particle> particles;

   public:
    /**
     * @brief Default constructor
     *
     * Generates an empty DirectSumContainer object.
     * If the amount of particles is known, it is recommended to use the constructor with the amount of particles as
     * parameter.
     */
    DirectSumContainer() = default;

    /**
     * @brief Constructor with amount of particles
     *
     * @param n Amount of particles
     *
     * Generates an empty DirectSumContainer object and reserves memory for the given amount of particles.
     */
    explicit DirectSumContainer(int n);

    /**
     * @brief Reserves memory for particles
     *
     * @param n Amount of particles
     *
     * Reserves memory for the given amount of particles.
     */
    void reserve(size_t n) override;

    /**
     * @brief Returns the amount of particles
     *
     * @return Amount of particles
     *
     * Returns the amount of particles in the container.
     */
    [[nodiscard]] std::size_t size() const override;

    /**
     * @brief Returns the capacity of the container
     *
     * @return Capacity of the container
     *
     * Returns the capacity of the container.
     */
    [[nodiscard]] std::size_t capacity() const;

    /**
     * @brief Adds a particle to the container
     *
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    void addParticle(const Particle& p) override;

    /**
     * @brief Adds a particle to the container
     *
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    void addParticle(Particle&& p) override;

    /**
     * @brief Returns a particle
     *
     * @param i Index of the particle
     * @return Particle
     *
     * Returns the particle at the given index in the internal data structure.
     */
    Particle& operator[](int i) override;

    /**
     * @brief Returns an iterator to the first particle
     *
     * @return Iterator to the first particle
     *
     * Returns the begin iterator for the internal data structure.
     */
    [[nodiscard]] std::vector<Particle>::iterator begin() override;

    /**
     * @brief Returns an end iterator for this container
     * @return End iterator for this container
     *
     * Returns the end iterator for the internal data structure.
     */
    [[nodiscard]] std::vector<Particle>::iterator end() override;

    /**
     * @brief Returns a const iterator to the first particle
     *
     * @return Const iterator to the first particle
     *
     * Returns the begin const iterator for the internal data structure.
     */
    [[nodiscard]] std::vector<Particle>::const_iterator begin() const override;

    /**
     * @brief Returns a const end iterator for this container
     *
     * @return Const end iterator for this container
     *
     * Returns the end const iterator for the internal data structure.
     */
    [[nodiscard]] std::vector<Particle>::const_iterator end() const override;

    /**
     * @brief Applies the given force sources to the particles
     *
     * @param force_sources Vector of force sources to be applied
     *
     * Applies the given force sources to the particles in the container.
     * Uses newton's third law to calculate the forces between the particles in a more optimized way.
     */
    void applyPairwiseForces(const std::vector<std::unique_ptr<ForceSource>>& force_sources) override;
};
