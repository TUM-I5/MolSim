#include "XSDTypeAdapter.h"

#include "io/logger/Logger.h"
#include "utils/ArrayUtils.h"
#include "utils/MaxwellBoltzmannDistribution.h"

CuboidSpawner XSDTypeAdapter::convertToCuboidSpawner(const particles::cuboid_spawner_type& cuboid) {
    auto lower_left_front_corner = convertToVector(cuboid.lower_left_front_corner());
    auto grid_dimensions = convertToVector(cuboid.grid_dim());
    auto initial_velocity = convertToVector(cuboid.velocity());

    auto grid_spacing = cuboid.grid_spacing();
    auto mass = cuboid.mass();
    auto type = cuboid.type();
    auto temperature = cuboid.temperature();

    if (grid_dimensions[0] <= 0 || grid_dimensions[1] <= 0 || grid_dimensions[2] <= 0) {
        Logger::logger->error("Cuboid grid dimensions must be positive");
        exit(-1);
    }

    if (grid_spacing <= 0) {
        Logger::logger->error("Cuboid grid spacing must be positive");
        exit(-1);
    }

    if (mass <= 0) {
        Logger::logger->error("Cuboid mass must be positive");
        exit(-1);
    }

    if (temperature < 0) {
        Logger::logger->error("Cuboid temperature must be positive");
        exit(-1);
    }

    return CuboidSpawner{lower_left_front_corner, grid_dimensions,        grid_spacing, mass,
                         initial_velocity,        static_cast<int>(type), temperature};
}

SphereSpawner XSDTypeAdapter::convertToSphereSpawner(const particles::sphere_spawner_type& sphere) {
    auto center = convertToVector(sphere.center());
    auto initial_velocity = convertToVector(sphere.velocity());

    auto radius = sphere.radius();
    auto grid_spacing = sphere.grid_spacing();
    auto mass = sphere.mass();
    auto type = sphere.type();
    auto temperature = sphere.temperature();

    if (radius <= 0) {
        Logger::logger->error("Sphere radius must be positive");
        exit(-1);
    }

    if (grid_spacing <= 0) {
        Logger::logger->error("Sphere grid spacing must be positive");
        exit(-1);
    }

    if (mass <= 0) {
        Logger::logger->error("Sphere mass must be positive");
        exit(-1);
    }

    if (temperature < 0) {
        Logger::logger->error("Sphere temperature must be positive");
        exit(-1);
    }

    return SphereSpawner{center, static_cast<int>(radius), grid_spacing, mass, initial_velocity, static_cast<int>(type), temperature};
}

Particle XSDTypeAdapter::convertToParticle(const particles::single_particle_type& particle) {
    auto position = convertToVector(particle.position());
    auto initial_velocity = convertToVector(particle.velocity()) + maxwellBoltzmannDistributedVelocity(particle.temperature(), 2);

    auto mass = particle.mass();
    auto type = particle.type();

    return Particle{position, initial_velocity, mass, static_cast<int>(type)};
}

std::variant<SimulationParams::DirectSumType, SimulationParams::LinkedCellsType> XSDTypeAdapter::convertToParticleContainer(
    const settings::particle_container_type& particle_container) {
    std::variant<SimulationParams::DirectSumType, SimulationParams::LinkedCellsType> container;

    if (particle_container.linkedcells_container().present()) {
        auto container_data = *particle_container.linkedcells_container();

        auto domain_size = XSDTypeAdapter::convertToVector(container_data.domain_size());
        auto cutoff_radius = container_data.cutoff_radius();

        container = SimulationParams::LinkedCellsType(domain_size, cutoff_radius);
    } else if (particle_container.directsum_container().present()) {
        container = SimulationParams::DirectSumType();
    } else {
        Logger::logger->error("Container type not implemented");
        exit(-1);
    }

    return container;
}

std::array<double, 3> XSDTypeAdapter::convertToVector(const DoubleVec3Type& vector) { return {vector.x(), vector.y(), vector.z()}; }

std::array<int, 3> XSDTypeAdapter::convertToVector(const IntVec3Type& vector) { return {vector.x(), vector.y(), vector.z()}; }