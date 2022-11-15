/*
 * ParticleContainer.h 
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/
#pragma once 

#include "Particle.h"

#include <vector>
#include <array>
#include <functional>

/**
 * @brief Wrapper for the particles in simulation, provides functions to iterate over the particles in different ways 
*/
class ParticleContainer {

private: 

    std::vector<Particle> particles; // particles in the simulation; we use a vector because even though creation takes longer, the iteration is much faster

public: 
    /**
     * @brief Iterates over all particles and applies the function f 
     * @param f A lambda function applied for every particle
    */
    const void iterateParticles(std::function<void(Particle&)> f); 

    /**
     * @brief Iterates over all pairs of particles and applies the function f 
     * @param f A lambda function applied for every pair of particles
     * @note Only iterates over each pair of particles once, so the function passed should handle if there needs to be something calculated for both particles
    */
    const void iterateParticlePairs(std::function<void(Particle&, Particle&)> f); 

    /**
     * @brief Creates a new particle and adds it to the vector
     * @param x The position array of the particle 
     * @param v The velocity array of the particle 
     * @param m The mass of the particle
    */
    const void addParticle(std::array<double, 3> &x, std::array<double, 3>  &v, double &m); 

    /**
     * @brief Returns the number of particles in the simulation
     * @return The size of the particle vector
    */
    const int size() const; 

    const void resetParticles(); 

    std::vector<Particle> &getParticles(); 
};