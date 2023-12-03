#include "SphereSpawner.h"

#include "particles/Particle.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

SphereSpawner::SphereSpawner(const std::array<double, 3>& center, const int sphere_radius, double grid_spacing, double mass,
                             const std::array<double, 3>& initial_velocity, int type, bool third_dimension, double avg_velocity)
    : center(center),
      sphere_radius(sphere_radius),
      grid_spacing(grid_spacing),
      mass(mass),
      type(type),
      initial_velocity(initial_velocity),
      avg_velocity(avg_velocity),
      third_dimension(third_dimension) {}

void SphereSpawner::spawnParticles(std::unique_ptr<ParticleContainer>& particle_container) const {
    for (int x = -sphere_radius; x <= sphere_radius; x++) {
        for (int y = -sphere_radius; y <= sphere_radius; y++) {
            for (int z = -sphere_radius; z <= sphere_radius; z++) {
                if (!third_dimension && z != 0) {
                    continue;
                }

                const auto displacement = std::array<double, 3>{x * grid_spacing, y * grid_spacing, z * grid_spacing};
                const double dist = ArrayUtils::L2Norm(displacement);

                if (dist <= sphere_radius * grid_spacing) {
                    const auto position = center + displacement;
                    const auto velocity = initial_velocity + maxwellBoltzmannDistributedVelocity(avg_velocity, third_dimension ? 3 : 2);

                    particle_container->addParticle(Particle(position, velocity, mass, type));
                }
            }
        }
    }
}

size_t SphereSpawner::getEstimatedNumberOfParticles() const { return static_cast<size_t>(4.0 / 3.0 * M_PI * std::pow(sphere_radius, 3)); }