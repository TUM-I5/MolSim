//
// Created by Feryal Ezgi on 16.11.2023.
//

#include "Generator.h"
#include "ArrayUtils.h"
#include <array>


void Generator::cuboid(ParticleContainer &container, std::array<double, 3> position, std::array<int, 3> size,
                       double meshWidth, std::array<double, 3> velocity, double mass, int typeId) {
    for (int x = 0; x < size[0]; x++) {
        for (int y = 0; y < size[1]; y++) {
            for (int z = 0; z < size[2]; z++) {
                container.add(Particle{{
                                               position[0] + x * meshWidth,
                                               position[1] + y * meshWidth,
                                               position[2] + z * meshWidth
                                       },
                                       velocity,
                                       mass,
                                       typeId}
                );
            }
        }
    }
}

// Iterate over a cubic area around the sphere with the given parameters and add a particle to container if it is inside the sphere boundaries
void Generator::sphere(ParticleContainer &container, std::array<double, 3> center, int radius, double meshWidth,
                       std::array<double, 3> velocity, double mass, int typeId) {

    // Distance from the center to the edge of the sphere
    double dis = radius * meshWidth;
    // Calculate the bounds for iteration
    double minBoundX = center[0] - dis;
    double maxBoundX = center[0] + dis;
    double minBoundY = center[1] - dis;
    double maxBoundY = center[1] + dis;
    double minBoundZ = center[2] - dis;
    double maxBoundZ = center[2] + dis;

    // Iterate over the cubic area around the sphere and add particles
    for (double x = minBoundX; x <= maxBoundX; x += meshWidth) {
        for (double y = minBoundY; y <= maxBoundY; y += meshWidth) {
            for (double z = minBoundZ; z <= maxBoundZ; z += meshWidth) {
                std::array<double, 3> position = {x, y, z};
                double normalizedDistance = ArrayUtils::L2Norm(position - center) / meshWidth;

                // Check if the particle is inside the sphere boundaries
                if (normalizedDistance <= radius) {
                    container.add(Particle{position, velocity, mass, typeId});
                }
            }
        }
    }
}






