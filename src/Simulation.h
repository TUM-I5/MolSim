//
// Created by alex on 30.10.2022.
//

#pragma once

#include "Particle.h"

namespace sim {
    /**
    * calculate the force for all particles
    */
    void calculateF();

    /**
    * calculate the position for all particles
    */
    void calculateX();

    /**
    * calculate the position for all particles
    */
    void calculateV();

    /**
    * @brief Adds the Force between two Particles to their respective force-parameters
    *
    * @param p1
    * @param p2
    */
    void forceBetw2Particles(Particle &p1, Particle &p2);
} // sim