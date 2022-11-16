/*
 * CuboidInputReader.h
 *
 *  Created on: 16.11.2022
 *      Author: borisov
 */

#pragma once

#include "../model/Cuboid.h"
#include "../model/ParticleContainer.h"

/**
 * @brief Class that takes a Cuboid and creates Particles representing that cuboid
 */
class CuboidInputReader {
public:
    /**
     * @brief Creates particles representing a cuboid
     * 
     * @param particleContainer The container in which the created particles will be stored
     * @param cuboid The cuboid for which the particles will be created 
     */
    void readInput(ParticleContainer& particleContainer, Cuboid& cuboid);
};
