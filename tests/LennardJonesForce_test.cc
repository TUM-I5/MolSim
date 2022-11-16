#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include "../src/model/ParticleContainer.h"
#include "../src/utils/ArrayUtils.h"
#include "../src/simulation/LennardJonesSimulation.h"

//check correctness of Lennard-Jones-Forcecalculation against hand-calculated values
TEST(LennardJonesForce, DistanceOf1) {
    ParticleContainer pc = ParticleContainer();
    std::array<double,3> x1 = {1,0,0};
    std::array<double,3> x2 = {0,0,0};
    std::array<double,3> v = {0,0,0};
    double m = 1;
    pc.addParticle(x1,v,m);
    pc.addParticle(x2,v,m);

    double end_time = 0; //calculateF() is called one time before loop begins, therefore no timesteps are needed
    double delta_t = 1;

    //calculating new forces according to Lennard-Jones potential with hardcoded values epsilon=5 and sigma=1
    std::unique_ptr<Simulation> simulation = std::make_unique<LennardJonesSimulation>(LennardJonesSimulation(&pc, end_time, delta_t));
    simulation->simulate();

    std::vector<Particle> &particles = pc.getParticles(); //is & correct here?

    EXPECT_THAT(particles[0].getF(), testing::ElementsAre(120,0,0));
    EXPECT_THAT(particles[1].getF(), testing::ElementsAre(-120,0,0));
}