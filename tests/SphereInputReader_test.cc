#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/DirectSumParticleContainer.h"
#include "../src/inputReader/SphereInputReader.h"

// returns number points between lower and upper in plain marked with index
int getNumberOfPointsBetween(double lower, double upper, int index, std::shared_ptr<ParticleContainer> pc)
{
    int count = 0;
    for (auto &p : pc->getActiveParticles())
    {
        if (p.getX()[index] >= lower && p.getX()[index] <= upper)
        {
            count++;
        }
    }
    return count;
}

// correctness of symmetry of sphere
TEST(SphereInputReader, SphereSymmetry)
{
    std::shared_ptr<ProgramParameters> pp = std::make_shared<ProgramParameters>();
    std::unique_ptr<SphereInputReader> sph = std::make_unique<SphereInputReader>(SphereInputReader());
    // h = 1; center at 0,0,0, r = 20
    const char *file = "../tests/eingabe-sphere.txt";
    sph->readInput(*pp, file);

    int leftOfCenter = getNumberOfPointsBetween(0, 20, 0, pp->getParticleContainer());
    int rightOfCenter = getNumberOfPointsBetween(20, 40, 0, pp->getParticleContainer());
    int bottomOfCenter = getNumberOfPointsBetween(0, 20, 1, pp->getParticleContainer());
    int topOfCenter = getNumberOfPointsBetween(20, 40, 1, pp->getParticleContainer());

    EXPECT_EQ(leftOfCenter, rightOfCenter);
    EXPECT_EQ(leftOfCenter, topOfCenter);
    EXPECT_EQ(bottomOfCenter, rightOfCenter);
}