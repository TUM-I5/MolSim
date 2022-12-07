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

// returns position of smallest point in plain marked with index
double findSmallestPoint(int index, std::shared_ptr<ParticleContainer> pc)
{
    double smallest;
    for (auto &p : pc->getActiveParticles())
    {
        if (p.getX()[index] <= smallest)
        {
            smallest = p.getX()[index];
        }
    }
    return smallest;
}

// returns position of biggest point in plain marked with index
double findBiggestPoint(int index, std::shared_ptr<ParticleContainer> pc)
{
    double biggest;
    for (auto &p : pc->getActiveParticles())
    {
        if (p.getX()[index] >= biggest)
        {
            biggest = p.getX()[index];
        }
    }
    return biggest;
}

// correctness of symmetry of sphere
TEST(SphereInputReader, SphereSymmetry)
{
    std::shared_ptr<ParticleContainer> pc;
    pc.reset(new DirectSumParticleContainer());
    std::unique_ptr<SphereInputReader> sph = std::make_unique<SphereInputReader>(SphereInputReader());
    // h = 1; center at 0,0,0, r = 20
    const char *file = "../tests/eingabe-sphere-off-zero.txt";
    sph->readInput(*pc, file);

    int leftOfCenter = getNumberOfPointsBetween(-25, -5.5, 0, pc);
    int rightOfCenter = getNumberOfPointsBetween(-4.5, 15, 0, pc);
    int bottomOfCenter = getNumberOfPointsBetween(-25, -5.5, 1, pc);
    int topOfCenter = getNumberOfPointsBetween(-4.5, 15, 1, pc);

    EXPECT_EQ(leftOfCenter, rightOfCenter);
    EXPECT_EQ(leftOfCenter, topOfCenter);
    EXPECT_EQ(bottomOfCenter, rightOfCenter);
}

// c orrectness of outer boundaries of sphere
TEST(SphereInputReader, SphereBoundaries)
{
    std::shared_ptr<ParticleContainer> pc;
    pc.reset(new DirectSumParticleContainer());
    std::unique_ptr<SphereInputReader> sph = std::make_unique<SphereInputReader>(SphereInputReader());
    // h = 1; center at 0,0,0, r = 20
    const char *file = "../tests/eingabe-sphere.txt";
    sph->readInput(*pc, file);

    int leftMost = findSmallestPoint(0, pc);
    int rightMost = findBiggestPoint(0, pc);
    int bottomMost = findSmallestPoint(1, pc);
    int topMost = findBiggestPoint(1, pc);

    EXPECT_EQ(leftMost, (-rightMost));
    EXPECT_EQ(rightMost, topMost);
    EXPECT_EQ(topMost, (-bottomMost));
}