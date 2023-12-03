//
// Created by Alp Kaan Aksu on 04.11.23.
// Updated by Feryal Ezgi Aşkın on 19.11.23

#pragma once

#include "models/Particle.h"
#include "models/ParticleContainer.h"

namespace Generator {
    /**
     * @brief Generates a cuboid of particles
     *
     * @param container
     * @param position
     * @param size
     * @param meshWidth
     * @param velocity
     * @param mass
     * @param typeId
     */
    void cuboid(ParticleContainer &container, std::array<double, 3> position, std::array<double, 3> size, double meshWidth,
           std::array<double, 3> velocity, double mass, int typeId);

    /**
     * @brief Generates a sphere of particles
     * @param container
     * @param center
     * @param radius
     * @param meshWidth
     * @param velocity
     * @param mass
     * @param typeId
     */
    void sphere(ParticleContainer &container, std::array<double, 3> center, int radius, double meshWidth,
                std::array<double, 3> velocity, double mass, int typeId);

    /**
     * @brief Generates a disk of particles
     * @note Equivalent to a '2d sphere'
     * @param container
     * @param center
     * @param radius
     * @param meshWidth
     * @param velocity
     * @param mass
     * @param typeId
     */
    void disk(ParticleContainer &container, std::array<double, 3> center, int radius, double meshWidth,
                std::array<double, 3> velocity, double mass, int typeId);

    void addParticleIfInsideSphere(ParticleContainer &container, std::array<double, 3> position,
            std::array<double, 3> velocity, double mass, int typeId,
            std::array<double, 3> center, int radius, double meshWidth);

}
