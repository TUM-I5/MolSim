#include "CuboidGenerator.h"

void CuboidGenerator::generateCuboid(FileReader::CuboidData& cuboid, ParticleContainer& particleContainer) {
    for(int z = 0; z < cuboid.N3; z++) {

        for (int y = 0; y < cuboid.N2; y++) {

            for (int x = 0; x < cuboid.N1; x++) {
                std::array<double, 3> cords(cuboid.x);
                //todo calculate velocity
                std::array<double, 3> vel(cuboid.v);

                cords[0] += x * cuboid.h;
                cords[1] += y * cuboid.h;
                cords[2] += z * cuboid.h;

                particleContainer.addParticle(cords, vel, cuboid.m);
            }
        }
    }
}



