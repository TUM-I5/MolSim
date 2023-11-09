#include "CuboidSpawner.h"

#include "types/Particle.h"
#include "utils/ArrayUtils.h"

CuboidSpawner::CuboidSpawner(std::array<double, 3>& lower_left_corner, std::array<uint, 3>& grid_dimensions, double grid_spacing, double mass, std::array<double, 3>& initial_velocity, int type) {
    this->lower_left_corner = lower_left_corner;
    this->grid_dimensions = grid_dimensions;
    this->grid_spacing = grid_spacing;
    this->mass = mass;
    this->initial_velocity = initial_velocity;
    this->type = type;
}

void CuboidSpawner::spawnParticles(ParticleContainer& particle_container) {
    for (uint i = 0; i < grid_dimensions[0]; i++) {
        for (uint j = 0; j < grid_dimensions[1]; j++) {
            for (uint k = 0; k < grid_dimensions[2]; k++) {
                std::array<double, 3> x = lower_left_corner + grid_spacing * std::array<double, 3>({static_cast<double>(i), static_cast<double>(j), static_cast<double>(k)});

                Particle particle = Particle(x, initial_velocity, mass, type);
                particle_container.addParticle(std::move(particle));
            }
        }
    }
}