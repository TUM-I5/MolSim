//
// Created by Feryal Ezgi on 16.11.2023.
//

#include "Generator.h"
#include <cmath>
#include <array>


void Generator::cuboid(ParticleContainer &container, std::array<double, 3> position, std::array<double, 3> size,
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

// Checks if a given position is inside the sphere defined by the parameters position
void Generator::addParticleIfInsideSphere(ParticleContainer &container, std::array<double, 3> position, std::array<double, 3> velocity,
                                          double mass, int typeId, std::array<double, 3> center, int radius, double meshWidth) {
    // normalized distance between the particle and the center in each dimension
    // squared to simplify comparison
    double distanceSquared = std::pow((position[0] - center[0]) / meshWidth, 2) +
                             std::pow((position[1] - center[1]) / meshWidth, 2) +
                             std::pow((position[2] - center[2]) / meshWidth, 2);

    if (distanceSquared <= std::pow(radius, 2)) {
        container.add(Particle{position, velocity, mass, typeId});
    }
}

// Iterate over a cubic area around the sphere with the given parameters and add a particle to container if it is inside the sphere boundaries
void Generator::sphere(ParticleContainer &container, std::array<double, 3> center, int radius, double meshWidth,
                       std::array<double, 3> velocity, double mass, int typeId) {

    // distance from the center to the edge of the sphere
    double dis = radius * meshWidth;
    for (double x = center[0] - dis; x <= center[0] + dis; x += meshWidth) {
        for (double y = center[1] - dis; y <= center[1] + dis; y += meshWidth) {
            for (double z = center[2] - dis; z <= center[2] + dis; z += meshWidth) {
                addParticleIfInsideSphere(container, {x, y, z}, velocity, mass, typeId, center, radius, meshWidth);
            }
        }
    }
}






