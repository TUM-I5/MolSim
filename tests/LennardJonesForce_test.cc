#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "../src/model/DirectSumParticleContainer.h"
#include "../src/utils/ArrayUtils.h"
#include "../src/simulation/LennardJonesForce.h"

// check correctness of Lennard-Jones-Forcecalculation against hand-calculated values
TEST(LennardJonesForce, DistanceOf1)
{
    DirectSumParticleContainer pc = DirectSumParticleContainer();
    std::array<double, 3> x1 = {1, 0, 0};
    std::array<double, 3> x2 = {0, 0, 0};
    std::array<double, 3> v = {0, 0, 0};
    double m = 1;
    double sigma = 1;
    double epsilon = 5;
    pc.addParticle(x1, v, m, epsilon, sigma);
    pc.addParticle(x2, v, m, epsilon, sigma);

    double end_time = 0; // calculateF() is called one time before loop begins, therefore no timesteps are needed
    double delta_t = 1;

    // calculating new forces according to Lennard-Jones potential with hardcoded values epsilon=5 and sigma=1
    std::unique_ptr<ForceCalculation> calculation = std::make_unique<LennardJonesForce>();

    calculation->calculateForce(pc);

    std::vector<Particle> &particles = pc.getActiveParticles();

    EXPECT_THAT(particles[0].getF(), testing::ElementsAre(120, 0, 0));
    EXPECT_THAT(particles[1].getF(), testing::ElementsAre(-120, 0, 0));
}