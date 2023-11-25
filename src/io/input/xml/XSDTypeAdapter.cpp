#include "XSDTypeAdapter.h"

#include "io/logger/Logger.h"

template <typename T>
bool isNegative_vector(std::array<T, 3>& arr) {
    return arr[0] < 0 || arr[1] < 0 || arr[2] < 0;
}

CuboidSpawner XMLTypeAdapter::convertToCuboidSpawner(const ::cuboid& cuboid) {
    auto pos = cuboid.position();
    auto dimensions = cuboid.grid_dim();
    auto grid_spacing = cuboid.grid_spacing();
    auto mass = cuboid.mass();
    auto velocity = cuboid.velocity();
    auto type = cuboid.type();
    auto temperature = cuboid.temperature();

    std::array<double, 3> lower_left_front_corner{pos.x(), pos.y(), pos.z()};
    std::array<int, 3> grid_dimensions{dimensions.dimx(), dimensions.dimy(), dimensions.dimz()};
    std::array<double, 3> initial_velocity{velocity.x(), velocity.y(), velocity.z()};

    if (isNegative_vector(grid_dimensions)) {
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

    if (type < 0) {
        Logger::logger->error("Cuboid type must be positive");
        exit(-1);
    }

    return CuboidSpawner(lower_left_front_corner, grid_dimensions, grid_spacing, mass, initial_velocity, type, temperature);
}