#pragma once

#include <array>

#include "ParticleSpawner.h"

/**
 * @brief Class to spawn particles in a cuboid. Implements the interface ParticleSpawner.
 *
 * Implementation of the particle spawning in a cuboid.
 * It is possible to specify the lower left corner of the cuboid, the grid dimensions, the spacing between neighboring particles, the mass of the particles, the
 * initial velocity of the particles and the type of the particles.
 */
class CuboidSpawner : public ParticleSpawner {
   private:
    const std::array<double, 3> lower_left_corner;
    const std::array<int, 3> grid_dimensions;
    const double grid_spacing;

    const double mass;

    const int type;

    const std::array<double, 3> initial_velocity;
    const double avg_velocity;

   public:
    /**
     * @brief Constructor
     * @param lower_left_corner Lower left corner of the cuboid
     * @param grid_dimensions Grid dimensions
     * @param grid_spacing Spacing between neighboring particles
     * @param mass Mass of the particles
     * @param initial_velocity Initial velocity of the particles, the velocity is then jittered by a Maxwell-Boltzmann distribution
     * @param type Type of the particles in the cuboid
     * @param avg_velocity Average brownian motion velocity of the particles
     *
     * Constructor to initialize the cuboid spawner. The velocity of the particles is jittered by a Maxwell-Boltzmann distribution.
     */
    CuboidSpawner(const std::array<double, 3>& lower_left_corner, const std::array<int, 3>& grid_dimensions, double grid_spacing, double mass,
                  const std::array<double, 3>& initial_velocity, int type, double avg_velocity = 0.1);

    /**
     * @brief Spawns particles in the given container
     * @param particle_container Container to spawn particles in
     *
     * Spawns particles in the given container.
     */
    void spawnParticles(ParticleContainer& particle_container) const override;
};