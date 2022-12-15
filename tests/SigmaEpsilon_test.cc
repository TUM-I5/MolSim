#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/LinkedCellParticleContainer.h"
#include "../src/simulation/LennardJonesForce.h"
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

TEST(SigmaEpsilon, MixedVariForce){
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9, 9, 9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out};
    LennardJonesForce forceCalc = LennardJonesForce();
    ParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries); 
    std::array<double, 3> x1 = {1, 0, 0};
    std::array<double, 3> x2 = {0, 0, 0};
    std::array<double, 3> v = {0, 0, 0};
    double m = 1;
    double sigma_1 = 1;
    double sigma_2 = 3; 
    double epsilon_1 = 1; 
    double epsilon_2 = 4; 
    pc.addParticle(x1, v, m, epsilon_1, sigma_1);
    pc.addParticle(x2, v, m, epsilon_2, sigma_2);

    // calculating new forces according to Lennard-Jones potential with hardcoded values epsilon=5 and sigma=1
    LennardJonesForce calculation = LennardJonesForce();

    calculation.calculateForce(pc);

    std::vector<Particle> &particles = pc.getActiveParticles();

    EXPECT_THAT(particles[0].getF(), testing::ElementsAre(390144, 0, 0));
    EXPECT_THAT(particles[1].getF(), testing::ElementsAre(-390144, 0, 0));
}