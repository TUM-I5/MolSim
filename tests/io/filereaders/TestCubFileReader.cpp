#include <gtest/gtest.h>

#include "io/input/CubFileReader.h"
#include "particles/ParticleContainer.h"
#include "utils/ArrayUtils.h"

#include <algorithm>

#define EXPECT_CONTAINS_POS_NEAR(list, point, tol) \
    EXPECT_TRUE(std::find_if(list.begin(), list.end(), [&](auto& x) { return ArrayUtils::L2Norm(x - point) < tol; }) != list.end());

/*
 * Test if a CubFileReader reads the correct data out of the files.
 */
TEST(CubFileReader, CorrectReadingOfParticles) {
    ParticleContainer particle_container;
    CubFileReader cub_file_reader;
    cub_file_reader.readFile(std::string(TESTS_SRC_DIR) + "/io/inputfiles/CubExample.cub", particle_container);

    EXPECT_EQ(particle_container.size(), 11 * 12 * 13 + 8 * 4 * 1);

    EXPECT_NEAR(particle_container[0].getM(), 3.14, 1e-13);
    EXPECT_EQ(particle_container[0].getType(), 42);

    EXPECT_NEAR(particle_container[particle_container.size() - 1].getM(), 1.0, 1e-13);
    EXPECT_EQ(particle_container[particle_container.size() - 1].getType(), 112);
}