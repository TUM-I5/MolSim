#pragma once

#include <array>

#include "ParticleSpawner.h"

/**
 * @brief Class to spawn particles in a cuboid. Implements the interface ParticleSpawner.
 *
 * Implementation of the particle spawning in a cuboid.
 * It is possible to specify the lower left corner of the cuboid, the grid dimensions, the spacing between neighboring particles, the mass of the particles, the initial velocity of the particles and the type of the particles.
 */
class CuboidSpawner : public ParticleSpawner {
   private:
    /**
     * @brief Lower left corner of the cuboid
     */
    std::array<double, 3> lower_left_corner;

    /**
     * @brief Grid Dimensions
     */
    std::array<uint, 3> grid_dimensions;

    /**
     * @brief spacing between neighboring particles
     */
    double grid_spacing;

    /**
     * @brief Mass of the particles
     */
    double mass;

    /**
     * @brief Initial velocity of the particles
     */
    std::array<double, 3> initial_velocity;

    /**
     * @brief Type of the particles
     */
    int type;

   public:
    /**
     * @brief Constructor
     * @param lower_left_corner Lower left corner of the cuboid
     * @param grid_dimensions Grid dimensions
     * @param grid_spacing Spacing between neighboring particles
     * @param initial_velocity Initial velocity of the particles, the velocity is then jittered by a Maxwell-Boltzmann distribution
     *
     * Constructor to initialize the cuboid spawner. The velocity of the particles is jittered by a Maxwell-Boltzmann distribution.
     */
    CuboidSpawner(std::array<double, 3>& lower_left_corner, std::array<uint, 3>& grid_dimensions, double grid_spacing, double mass, std::array<double, 3>& initial_velocity, int type = 0);

    /**
     * @brief Spawns particles in the given container
     * @param particle_container Container to spawn particles in
     *
     * Spawns particles in the given container.
     */
    void spawnParticles(ParticleContainer& particle_container) override;
};