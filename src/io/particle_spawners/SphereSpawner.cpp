#include "SphereSpawner.h"

#include "particles/Particle.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

SphereSpawner::SphereSpawner(const std::array<double, 3>& center, const int sphere_radius, double grid_spacing, double mass,
                             const std::array<double, 3>& initial_velocity, int type, double avg_velocity)
    : center(center),
      sphere_radius(sphere_radius),
      grid_spacing(grid_spacing),
      mass(mass),
      type(type),
      initial_velocity(initial_velocity),
      avg_velocity(avg_velocity) {}

void SphereSpawner::spawnParticles(ParticleContainer& particle_container) const {
    for (int x = -sphere_radius; x <= sphere_radius; x++) {
        for (int y = -sphere_radius; y <= sphere_radius; y++) {
            for (int z = -sphere_radius; z <= sphere_radius; z++) {
                const auto displacement = std::array<double, 3>{x * grid_spacing, y * grid_spacing, z * grid_spacing};
                const double dist = ArrayUtils::L2Norm(displacement);

                if (dist <= sphere_radius * grid_spacing) {
                    const auto position = center + displacement;
                    const auto velocity = maxwellBoltzmannDistributedVelocity(avg_velocity, 2);

                    particle_container.addParticle(Particle(position, velocity, mass, type));
                }
            }
        }
    }
}