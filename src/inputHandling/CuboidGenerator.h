#pragma once

#include "particleModel/ParticleContainer.h"
#include "FileReader.h"
#include "utils/MaxwellBoltzmannDistribution.h"

void generateCuboid(FileReader::CuboidData& cuboid, ParticleContainer& particleContainer) {

    for(uint64_t z = 0; z < cuboid.N3; z++) {

        for (uint64_t y = 0; y < cuboid.N2; y++) {

            for (uint64_t x = 0; x < cuboid.N1; x++) {
                std::array<double, 3> cords(cuboid.x);
                std::array<double, 3> vel(cuboid.v);
                std::array<double, 3> dist(maxwellBoltzmannDistributedVelocity(cuboid.avg_v, 3));

                cords[0] += x * cuboid.h;
                cords[1] += y * cuboid.h;
                cords[2] += z * cuboid.h;

                vel[0] += dist[0];
                vel[1] += dist[1];
                vel[2] += dist[2];
                
                particleContainer.addParticle(cords, vel, cuboid.m);
                
            }
        }
    }
}

void addCuboids(ParticleContainer& particleContainer, std::list<FileReader::CuboidData> cuboids) {
    //determine total amount of particles that will be generated
    size_t needed_capacity = 0;
    for (auto &cube : cuboids) {
        needed_capacity += (cube.N1 * cube.N2 * cube.N3);
    }

    //allocate the needed amount of memory in the beginning
    //to avoid copying with shrink_to_fit
    particleContainer.reserve(needed_capacity);

    std::cout << "The following cuboids were read: \n" << std::endl;
    for (auto &cube : cuboids) {
        std::cout << cube.to_string() << std::endl;
        generateCuboid(cube, particleContainer);
    }
}