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
     *  @brief Iterator class for ParticleContainer
     *
     * This class implements an iterator for the ParticleContainer class.
     */
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Particle;
        using pointer = Particle*;
        using reference = Particle&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator& operator++() {
            m_ptr++;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

       private:
        pointer m_ptr;
    };

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
    std::size_t size();

    /**
     * @brief Adds a particle to the container
     * @param p Particle to be added
     *
     * Adds a particle to the container.
     */
    void addParticle(Particle& p);

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
     * Returns an iterator to the first particle in the container.
     */
    Iterator begin();

    /**
     * @brief Returns an end iterator for this container
     * @return End iterator for this container
     *
     * Returns an iterator to the first memory slot after the last particle in this container.
     * Please note that this iterator is not dereferencable, and should just be used to compare to other iterators.
     */
    Iterator end();
};
