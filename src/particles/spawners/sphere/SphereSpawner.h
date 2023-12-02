#pragma once

#include <array>

#include "particles/spawners/ParticleSpawner.h"

/**
 * @brief Class to spawn particles in a Sphere. Implements the interface ParticleSpawner.
 *
 * Implementation of the particle spawning in a cuboid.
 * It is possible to specify the center of the sphere, the radius of the sphere, the spacing between neighboring particles, the mass
 * of the particles, the initial velocity of the particles and the type of the particles.
 *
 * The radius is given in terms of number of particles along the radius.
 */
class SphereSpawner : public ParticleSpawner {
   private:
    const std::array<double, 3> center;
    const int sphere_radius;
    const double grid_spacing;

    const double mass;
    const int type;

    const std::array<double, 3> initial_velocity;
    const double avg_velocity;

    const bool third_dimension;

   public:
    /**
     * @brief Constructor
     * @param center Center of the sphere
     * @param sphere_radius Radius of the sphere [number of particles along the radius]
     * @param grid_spacing Spacing between neighboring particles
     * @param mass Mass of the particles
     * @param initial_velocity Initial velocity of the particles, the velocity is then jittered by a Maxwell-Boltzmann distribution
     * @param type Type of the particles in the cuboid
     * @param avg_velocity Average brownian motion velocity of the particles
     * @param dimensions Number of dimensions of the sphere
     *
     * Constructor to initialize the cuboid spawner. The velocity of the particles is jittered by a Maxwell-Boltzmann distribution.
     */
    SphereSpawner(const std::array<double, 3>& center, int sphere_radius, double grid_spacing, double mass,
                  const std::array<double, 3>& initial_velocity, int type, bool third_dimension = true, double avg_velocity = 0.1);

    /**
     * @brief Spawns particles in the given container
     * @param particle_container Container to spawn particles in
     *
     * Spawns particles in the given container.
     */
    void spawnParticles(std::unique_ptr<ParticleContainer>& particle_container) const override;

    /**
     * @brief Returns the number of particles to be spawned
     * @return Number of particles to be spawned
     *
     * Returns the number of particles to be spawned.
     */
    [[nodiscard]] size_t getEstimatedNumberOfParticles() const override;
};