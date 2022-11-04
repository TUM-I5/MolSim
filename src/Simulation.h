//
// Created by alex on 30.10.2022.
//

#pragma once

#include "Particle.h"
#include "ParticleContainer.h"

namespace sim {
    extern ParticleContainer particleContainer;
    extern double start_time;
    extern double end_time;
    extern double delta_t;

    /**
    * calculate the force for all particles by gravitation.
    */
    void calculateFGravity();

    /**
    * calculate the position for all particles using the Stoermer Velvet method.
    */
    void calculateXStoermerVelvet();

    /**
    * calculate the position for all particles using the Stoermer Velvet method.
    */
    void calculateVStoermerVelvet();

    /**
    * @brief Adds the Force between two Particles to their respective force-parameters
    *
    * @param p1
    * @param p2
    */
    void forceBetw2Particles(Particle &p1, Particle &p2);

    /**
     * Wrapper for the actually used implementations during the simulation for the different calculation methods.
     * */
    template<void calcF() = calculateFGravity, void calcX() = calculateXStoermerVelvet, void calcV() = calculateVStoermerVelvet>
    class Simulation{
    public:
        void calculateF() { calcF(); }
        void calculateX() { calcX(); }
        void calculateV() { calcV(); }
    };
} // sim