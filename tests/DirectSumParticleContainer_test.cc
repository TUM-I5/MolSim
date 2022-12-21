#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/DirectSumParticleContainer.h"
#include "../src/utils/ArrayUtils.h"

void init_test_pc(DirectSumParticleContainer &pc, int numParticles)
{
    std::array<double, 3> x = {0, 0, 0};
    std::array<double, 3> v = {1, 0, 0};
    double m = 1;
    double epsilon = 1;
    double sigma = 1;
    pc.reserveMemoryForParticles(4);
    for (int i = 0; i < numParticles; i++)
    {
        pc.addParticle(x, v, m, epsilon, sigma);
    }
}

/**
 * test correct iteration over all particles by changing their force attribute
 */
TEST(DirectSumParticleContainer, IterateParticles)
{
    DirectSumParticleContainer pc = DirectSumParticleContainer();
    init_test_pc(pc, 4);

    EXPECT_EQ(pc.size(), 4);

    std::array<double, 3> testF = {1, 2, 3};

    pc.iterateParticles([testF](Particle &p) -> void
                        { p.setF(testF); });

    std::vector<Particle> &particles = pc.getActiveParticles();

    for (auto &p : particles)
    {
        EXPECT_THAT(p.getF(), testing::ElementsAre(1, 2, 3));
    }
}
/**
 * test correct iteration over particle pairs (with Newton's 3rd law)
 * by comparing added velocities to hand calculated values
 */
TEST(DirectSumParticleContainer, IterateParticleInteractions)
{
    DirectSumParticleContainer pc = DirectSumParticleContainer();
    init_test_pc(pc, 4);
    EXPECT_EQ(pc.size(), 4);

    // only called once per pair
    pc.iterateParticleInteractions(
        [](Particle &p1, Particle &p2) -> void
        { p2.setV(p1.getV() + p2.getV()); });

    std::vector<Particle> &particles = pc.getActiveParticles();

    for (int i = 0; i < 4; i++)
    {
        EXPECT_THAT(particles[i].getV(), testing::ElementsAre(pow(2, i), 0, 0));
    }
}
