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
    extern double epsilon;
    extern double sigma;

    /**
    * calculate the force for all particles by gravitation.
    */
    void calculateFGravity();

    /**
     * calculate the force for all particles using the Lennard-Jones potential
     * */
     void calculateFLennardJones();

    /**
    * calculate the position for all particles using the Stoermer Velvet method.
    */
    void calculateXStoermerVelvet();

    /**
    * calculate the position for all particles using the Stoermer Velvet method.
    */
    void calculateVStoermerVelvet();

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