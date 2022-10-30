#pragma once

#include "ParticleContainer.h"
#include "Particle.h"

#include <vector>
#include <array>
#include <string>
#include <iterator>


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

    ParticleContainer();

    Iterator begin();

    Iterator end();

    /**
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
     * @brief Applies the given function to all Particles
     *
     * @param function
     */
    void forAllParticles(void (function)(Particle &));

    /**
     * @brief Applies given function to all pairs of Particles p_i, p_j, where p_i != p_j once. (If f(p_i, p_j) got invoked, f(p_j, p_i) won't get invoked with the same i and j)
     *
     * @param function
     */
    void forAllPairs(void (function)(Particle &p1, Particle &p2));

    /**
     * @brief Get the Particles object
     *
     * @return std::vector<Particle>
     */
    std::vector<Particle> &getParticles();

    struct Iterator {
    private:
        std::vector<Particle>::iterator it;
    public:
        explicit Iterator(std::vector<Particle>::iterator i) : it(i) {}

        Particle &operator*() { return *it; }

        Particle *operator->() { return it.operator->(); }

        Iterator &operator++() {
            it++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator &a, const Iterator &b) { return a.it == b.it; }

        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.it != b.it; }
    };
};