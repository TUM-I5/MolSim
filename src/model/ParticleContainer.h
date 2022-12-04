/*
 * ParticleContainer.h
 *
 * Created: 3.12.2022
 * Author:  marquardt
 */

#pragma once

#include "Particle.h"

/**
 * @brief abstract class for Particle Container 
 */
class ParticleContainer {

    protected:
        /**
         * a speedlog logger which logs construction and destruction of particles
         */
        std::shared_ptr<spdlog::logger> _memoryLogger;

    public:

        virtual ~ParticleContainer() {};

        /**
         * @brief Iterates over all active particles (inside domain) and applies the function f
         * @param f A lambda function applied for every particle
        */
        virtual const void iterateParticles(std::function<void(Particle &)> f) = 0;

        /**
         * @brief Computes interaction between two particles or a particle and a border
         * @param f A lambda function applied between particles or a particle and a border
        */
        virtual const void iterateParticleInteractions(std::function<void(Particle &, Particle &)> f) = 0;

        /**
         * @brief Constructs a new particle with the given parameters and adds it to the container 
         * @param x position array of the particle
         * @param v velocity array of the particle
         * @param m mass of the particle
        */
        virtual const void addParticle(std::array<double, 3> &x, std::array<double, 3> &v, double &m) = 0;

        /**
         * @brief returns the number of active particles
         * @return size of the particle vector
        */
        virtual const int size() const = 0;

        /**
         * @brief deletes all particles (active & halo) from the simulation
        */
        virtual const void resetParticles() = 0;

        /**
         * @brief reserves memory space for given number of particles to avoid constant resizing of the vectors
         * @param numberOfParticles number of additional particles to reserve space for
        */
        virtual const void reserveMemoryForParticles(int numberOfParticles) = 0;

        /**
         * @brief returns vector of active particles (particles inside domain boundaries)
         * @return number of active particles
        */
        virtual std::vector<Particle> &getActiveParticles() = 0;

};