#include "CuboidSpawner.h"

#include "particles/Particle.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

CuboidSpawner::CuboidSpawner(const std::array<double, 3>& lower_left_corner, const std::array<int, 3>& grid_dimensions, double grid_spacing,
                             double mass, const std::array<double, 3>& initial_velocity, int type, double avg_velocity)
    : lower_left_corner(lower_left_corner),
      grid_dimensions(grid_dimensions),
      grid_spacing(grid_spacing),
      mass(mass),
      type(type),
      initial_velocity(initial_velocity),
      avg_velocity(avg_velocity) {}

void CuboidSpawner::spawnParticles(std::unique_ptr<ParticleContainer>& particle_container) const {
    for (int i = 0; i < grid_dimensions[0]; i++) {
        for (int j = 0; j < grid_dimensions[1]; j++) {
            for (int k = 0; k < grid_dimensions[2]; k++) {
                const auto grid_pos = std::array<double, 3>{static_cast<double>(i), static_cast<double>(j), static_cast<double>(k)};

                const auto x = lower_left_corner + grid_spacing * grid_pos;
                const auto v = initial_velocity + maxwellBoltzmannDistributedVelocity(avg_velocity, 2);

                Particle particle(x, v, mass, type);
                particle_container->addParticle(std::move(particle));
            }
        }
    }
}

size_t CuboidSpawner::getEstimatedNumberOfParticles() const {
    return static_cast<size_t>(grid_dimensions[0]) * grid_dimensions[1] * grid_dimensions[2];
}