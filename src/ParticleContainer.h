#pragma once

#include "Particle.h"

#include <vector>
#include <array>


class ParticleContainer{
    private:
        std::vector<Particle> particles;
    public:
        /**
         * @brief return the amount of particles stored
         * 
         * @return int 
         */
        int size(){
            return particles.size();
        }

        ParticleContainer();

        /**
         * @brief Assumes that the given filename contains the particle details and creates a ParticleContainer
         * 
         * @param filename 
         */
        ParticleContainer(char* filename);

        /**
         * @brief Returns the particle at index i
         * 
         * @param i 
         */
        Particle& getParticle(int i);

        /**
         * @brief Applies the given function to all Particles
         * 
         * @param function 
         */
        void forAllParticles(void (function)(Particle&));

        /**
         * @brief Applies given function to all pairs of Particles p_i, p_j, where p_i != p_j once. (If f(p_i, p_j) got invoked, f(p_j, p_i) won't get invoked with the same i and j)
         * 
         * @param function 
         */
        void forAllPairs(void (function)(Particle& p1, Particle& p2));

        /**
         * @brief Get the Particles object
         * 
         * @return std::vector<Particle> 
         */
        std::vector<Particle> getParticles();
};