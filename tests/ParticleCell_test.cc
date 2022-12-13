#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/ParticleCell.h"
#include "../src/utils/ArrayUtils.h"

/**
 * test replacement of invalid particles when new particle is inserted
 */
TEST(ParticleCell, InsertParticle)
{
    BoundaryCondition outflow = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {outflow, outflow, outflow, outflow, outflow, outflow};
    ParticleCell cell = ParticleCell(CellType::InnerCell, boundaries);

    cell.reserveMemory(3);
    Particle p1 = Particle();
    Particle p2 = Particle();
    Particle p3 = Particle();

    cell.insertParticle(&p1);
    cell.insertParticle(&p2);

    EXPECT_EQ(cell.size(), 2); // particles should be added normally

    p2.setInvalid(true);
    cell.updateInvalidCounter();
    EXPECT_EQ(cell.size(), 1); // invalid particles are not counted

    cell.insertParticle(&p3);

    EXPECT_EQ(cell.size(), 2); // invalid particle p2 should be overwritten with p3
}

/**
 * test correct iteration over particle pairs (with Newton's 3rd law)
 * by comparing added velocities to hand calculated values
 */
TEST(ParticleCell, IterateParticlePairs)
{
    std::array<double, 3> x = {0, 0, 0};
    std::array<double, 3> v = {1, 0, 0};
    double m = 1;
    double epsilon = 1; 
    double sigma = 1; 

    Particle p1 = Particle(x, v, m, epsilon, sigma);
    Particle p2 = Particle(x, v, m, epsilon, sigma);
    Particle p3 = Particle(x, v, m, epsilon, sigma);
    Particle p4 = Particle(x, v, m, epsilon, sigma);

    BoundaryCondition outflow = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {outflow, outflow, outflow, outflow, outflow, outflow};
    ParticleCell cell = ParticleCell(CellType::InnerCell, boundaries);
    cell.reserveMemory(4);
    cell.insertParticle(&p1);
    cell.insertParticle(&p2);
    cell.insertParticle(&p3);
    cell.insertParticle(&p4);

    // only called once per pair
    cell.iterateParticlePairs([](Particle &p1, Particle &p2) -> void
                              { p2.setV(p1.getV() + p2.getV()); },
                              3.0);

    std::vector<Particle *> particles = cell.getCellParticles();

    for (int i = 0; i < 4; i++)
    {
        EXPECT_THAT(particles[i]->getV(), testing::ElementsAre(pow(2, i), 0, 0));
    }
}