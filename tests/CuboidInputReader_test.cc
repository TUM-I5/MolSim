#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/ParticleContainer.h"
#include "../src/model/DirectSumParticleContainer.h"
#include "../src/inputReader/CuboidInputReader.h"

//check correctness of CuboidInputReader
TEST(CuboidInputReader, ReadCuboid) {

    std::shared_ptr<ProgramParameters> pp = std::make_shared<ProgramParameters>();

    std::unique_ptr<CuboidInputReader> cub = std::make_unique<CuboidInputReader>(CuboidInputReader());

    // cuboid containing 64 particles
    const char *file = "../tests/eingabe-cuboid.txt";

    cub->readInput(*pp, file);
    EXPECT_TRUE(pp->getParticleContainer()->size() == 64);
}