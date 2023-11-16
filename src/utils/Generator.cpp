//
// Created by Feryal Ezgi on 16.11.2023.
//

#include "Generator.h"

void Generator::cuboid(ParticleContainer &container, std::array<double, 3> position, std::array<double, 3> size, double meshWidth, std::array<double, 3> velocity, double mass, int typeId) {
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