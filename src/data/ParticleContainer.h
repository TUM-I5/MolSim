#pragma once

#include "ParticleContainer.h"
#include "Particle.h"

#include <vector>
#include <array>
#include <string>
#include <iterator>

/**
 * @brief wrapper class that stores and manages access to the particles
 *      The given implementation is a wrapper class around a std::vector<Particle>
 *      Another implementation might decide to use a different underlying structure.
 */
class ParticleContainer {

private:
    std::vector<Particle> particles;
public:
    struct Iterator;

    /**
     * @brief return the amount of particles stored
     *
     * @return int
     */
    unsigned long size();

    /**
     * @brief Construct a new Particle Container object with no particles stored
     * 
     */
    ParticleContainer();

    Iterator begin();

    Iterator end();

    /**
     * @brief Construct a new Particle Container that stores the given particles
     * 
     * @param buffer for all particles. will be added to local storage.
     */
    ParticleContainer(const std::vector<Particle> &buffer);

    /**
     * @brief Returns the particle at index i
     *
     * @param i
     */
    Particle &getParticle(unsigned long i);

    /**
     * @brief Adds the Particle to buffer
     * 
     */
    void addParticle(const Particle&);

    /**
     * @brief Applies the given function to all Particles
     *
     * @param function
     */
    void forAllParticles(void (function)(Particle &));

    /**
     * @brief Applies given function to all pairs of Particles p_i, p_j, where p_i < p_j once
     *  (If f(p_i, p_j) got invoked, f(p_j, p_i) won't get invoked with the same i and j)
     * @param function
     */
    void forAllPairs(void (function)(Particle &p1, Particle &p2));

    /**
     * @brief Get the Particles object
     *
     * @return std::vector<Particle>
     */
    std::vector<Particle> &getParticles();

    /**
     * Removes all particles.
     * */
    void clear();

    /**
     * WIP: Temporary iterator for ParticleContainer. At the moment it simply delegates to std::vector::iterator
     * */
    struct Iterator {
    private:
        std::vector<Particle>::iterator it;
    public:
        /**
         * Constructor
         * */
        explicit Iterator(std::vector<Particle>::iterator i) : it(i) {}

        /**
         * Get reference
         * */
        Particle &operator*() { return *it; }

        /**
         * Get pointer
         * */
        Particle *operator->() { return it.operator->(); }

        /**
         * Pre Increment
         * */
        Iterator &operator++() {
            it++;
            return *this;
        }

        /**
         * Post Increment
         * */
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * Equals, delegates task.
         * */
        friend bool operator==(const Iterator &a, const Iterator &b) { return a.it == b.it; }

        /**
         * Not Equals, delegates task.
         * */
        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.it != b.it; }
    };
};