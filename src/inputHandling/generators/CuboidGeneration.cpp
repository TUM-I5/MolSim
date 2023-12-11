#include "CuboidGeneration.h"
#include "utils/MaxwellBoltzmannDistribution.h"

void generateCuboid(FileReader::CuboidData& cuboid, CellContainer& container, size_t dim) {

    for(uint64_t z = 0; z < cuboid.N3; z++) {

        for (uint64_t y = 0; y < cuboid.N2; y++) {

            for (uint64_t x = 0; x < cuboid.N1; x++) {
                std::array<double, 3> cords(cuboid.x);
                std::array<double, 3> vel(cuboid.v);
                std::array<double, 3> dist(maxwellBoltzmannDistributedVelocity(cuboid.avg_v, dim));

                cords[0] += x * cuboid.h;
                cords[1] += y * cuboid.h;
                cords[2] += z * cuboid.h;

                vel[0] += dist[0];
                vel[1] += dist[1];
                vel[2] += dist[2];

                container.addParticle(cords, vel, cuboid.m);

            }
        }
    }
}

void addCuboids(CellContainer& container, std::list<FileReader::CuboidData> cuboids) {
    size_t dim{2};
    double z = cuboids.front().x[2];
    for (auto &cube : cuboids) {
        if(1 < cube.N3 || cube.x[2] != z || cube.v[2] != 0) dim = 3;
    }

    for (auto &cube : cuboids) {
        generateCuboid(cube, container, dim);
    }
}