#include "CuboidSpawner.h"

#include "types/Particle.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

CuboidSpawner::CuboidSpawner(std::array<double, 3>& lower_left_corner, std::array<uint, 3>& grid_dimensions, double grid_spacing, double mass, std::array<double, 3>& initial_velocity, int type) : lower_left_corner(lower_left_corner), grid_dimensions(grid_dimensions), grid_spacing(grid_spacing), mass(mass), initial_velocity(initial_velocity), type(type) {}

void CuboidSpawner::spawnParticles(ParticleContainer& particle_container) {
    double avg_velocity = 0.1;

    for (uint i = 0; i < grid_dimensions[0]; i++) {
        for (uint j = 0; j < grid_dimensions[1]; j++) {
            for (uint k = 0; k < grid_dimensions[2]; k++) {
                std::array<double, 3> x = lower_left_corner + grid_spacing * std::array<double, 3>({static_cast<double>(i), static_cast<double>(j), static_cast<double>(k)});
                std::array<double, 3> v = initial_velocity + maxwellBoltzmannDistributedVelocity(avg_velocity, 3);

                Particle particle = Particle(x, v, mass, type);
                particle_container.addParticle(std::move(particle));
            }
        }
    }
}