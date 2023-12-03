#include <gtest/gtest.h>

#include "data/FileLoader.h"
#include "io/input/custom_formats/cub/CubFileReader.h"
#include "particles/containers/ParticleContainer.h"
#include "particles/containers/directsum/DirectSumContainer.h"
#include "utils/ArrayUtils.h"

#define EXPECT_CONTAINS_POS_NEAR(list, point, tol) \
    EXPECT_TRUE(std::find_if(list.begin(), list.end(), [&](auto& x) { return ArrayUtils::L2Norm(x - point) < tol; }) != list.end());

/*
 * Test if a CubFileReader reads the correct data out of the files.
 */
TEST(CubFileReader, CorrectReadingOfParticles) {
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();
    CubFileReader cub_file_reader;
    cub_file_reader.readFile(FileLoader::get_test_file_path("CubExample.cub"), particle_container);

    EXPECT_EQ(particle_container->size(), 11 * 12 * 13 + 8 * 4 * 1);

    EXPECT_NEAR((*particle_container)[0].getM(), 3.14, 1e-13);
    EXPECT_EQ((*particle_container)[0].getType(), 42);

    EXPECT_NEAR((*particle_container)[particle_container->size() - 1].getM(), 6.0, 1e-13);
    EXPECT_EQ((*particle_container)[particle_container->size() - 1].getType(), 112);
}