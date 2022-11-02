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
    std::vector<Particle> particles; /// particles in the simulation; we use a vector because even though creation takes longer, the iteration is much faster

public: 
    /**
     * Iterates over all particles and applies the function f 
     * @param std::function<void(Particle&)> f
    */
    const void iterateParticles(std::function<void(Particle&)> f); 

    /**
     * Iterates over all pairs of particles and applies the function f 
     * @param std::function<void(Particle&, Particle&)> f
    */
    const void iterateParticlePairs(std::function<void(Particle&, Particle&)> f); 

    /**
     * Creates a new particle and adds it to the vector
     * @param std::array<double, 3> &x, std::array<double, 3>  &v, double &m
    */
    const void addParticle(std::array<double, 3> &x, std::array<double, 3>  &v, double &m); 

    /**
     * Returns size of the underlying particle vector
     * @return int size
    */
    const int size() const; 

    std::vector<Particle> &getParticles(); 
};