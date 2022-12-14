#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/DirectSumParticleContainer.h"
#include "../src/inputReader/XMLInputReader.h"

// correctness of xml reading for simulation state
TEST(Checkpoints, ReadCheckpoint)
{
    std::shared_ptr<ProgramParameters> pp = std::make_shared<ProgramParameters>();
    std::unique_ptr<XMLInputReader> xml = std::make_unique<XMLInputReader>();

    const char *file = "../tests/SimulationState.xml";
    xml->readInput(*pp, file);
    Particle p = pp->getParticleContainer()->getActiveParticles()[0];
    EXPECT_THAT(p.getM(), 1);
    EXPECT_THAT(p.getEpsilon(), 2);
    EXPECT_THAT(p.getSigma(), 3);
    EXPECT_THAT(p.getType(), 4);
    std::array<double, 3> x;
    x = p.getX();
    EXPECT_THAT(x[0], 1);
    std::array<double, 3> v;
    v = p.getV();
    EXPECT_THAT(v[0], 2);
    std::array<double, 3> f;
    f = p.getF();
    EXPECT_THAT(f[0], 3);
    std::array<double, 3> old_f;
    old_f = p.getOldF();
    EXPECT_THAT(old_f[0], 4);
}