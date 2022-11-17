#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/ParticleContainer.h"
#include "../src/inputReader/CuboidInputReader.h"

//check correctness of CuboidInputReader
TEST(CuboidInputReader, ReadCuboid) {
    ParticleContainer pc = ParticleContainer();

    std::unique_ptr<CuboidInputReader> cub = std::make_unique<CuboidInputReader>(CuboidInputReader());

    // cuboid containing 64 particles
    const char *file = "../tests/eingabe-cuboid.txt";

    cub->readInput(pc, file);
    EXPECT_TRUE(pc.size() == 64);
}