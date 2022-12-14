#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/DirectSumParticleContainer.h"
#include "../src/inputReader/XMLInputReader.h"
#include "../src/inputReader/SphereInputReader.h"
#include "../src/inputReader/CuboidInputReader.h"

/**
 * Checks if the epsilon and sigma value is set correctly when reading in particles through multiple sources
 */
TEST(SigmaEpsilon, Parsing)
{
    std::shared_ptr<ProgramParameters> pp = std::make_shared<ProgramParameters>();
    std::unique_ptr<XMLInputReader> xml = std::make_unique<XMLInputReader>();
    std::unique_ptr<SphereInputReader> sph = std::make_unique<SphereInputReader>();
    std::unique_ptr<CuboidInputReader> cub = std::make_unique<CuboidInputReader>();

    const char *sim_file = "../tests/Simulation.xml";
    xml->readInput(*pp, sim_file);
    const char *cub_file = "../tests/eingabe-cuboid.txt";
    cub->readInput(*pp, cub_file); 
    const char *sph_file = "../tests/eingabe-sphere.txt";
    sph->readInput(*pp, sph_file);

    bool eps_sig = true;

    for (auto &p : pp->getParticleContainer()->getActiveParticles())
    {
        EXPECT_THAT(p.getSigma(), 1);
        EXPECT_THAT(p.getEpsilon(), 1);
    }
}