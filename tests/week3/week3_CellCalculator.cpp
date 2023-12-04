#include <gtest/gtest.h>
#include "particleModel/updating/CellCalculator.h"
#include "particleModel/updating/model.h"


/**
 * @brief
*/
TEST(cellcalculator, test_newOrder) {
    std::array<double,3> x111_a{0,0,0};
    std::array<double,3> x111_b{1,1,1};
    std::array<double,3> x112{1,1,4};
    std::array<double,3> v{0,0,0};
    double m = 1;

    //Build CellContainer
    CellContainer cellContainer{9.0,9.0,9.0,3.0,3.0};
    cellContainer.addParticle(x111_a,v, m);
    cellContainer.addParticle(x111_b,v, m);
    cellContainer.addParticle(x112,v, m);
    cellContainer.createPointers();
    CellCalculator cellCalculator{cellContainer,0.00005,"LennJones"};
    std::vector<std::vector<std::vector<std::vector<Particle*>>>>& particles = *cellContainer.getParticles();

    //Build ParticleContainer
    ParticleContainer particleContainer;
    particleContainer.addParticle(x111_a,v, m);
    particleContainer.addParticle(x111_b,v, m);
    particleContainer.addParticle(x112,v, m);
    Model model{particleContainer, "LennJones", 0.00005};

    //Run old order
    model.calculateF();
    model.shiftForces();

    model.calculateX();
    model.calculateF();
    model.calculateV();
    model.shiftForces();

    model.calculateX();
    model.calculateF();
    model.calculateV();
    model.shiftForces();

    model.calculateX();

    //Run new order
    cellCalculator.initializeFX();

    cellCalculator.calculateLinkedCellF();
    cellCalculator.calculateWithinFVX();

    cellCalculator.calculateLinkedCellF();
    cellCalculator.calculateWithinFVX();

    //compare particles
    Particle p1_old = *particleContainer.getNextParticle();
    Particle p2_old = *particleContainer.getNextParticle();
    Particle p3_old = *particleContainer.getNextParticle();

    Particle p1_new = *particles[1][1][1][0];
    Particle p2_new = *particles[1][1][1][1];
    Particle p3_new = *particles[1][1][2][0];

    ASSERT_EQ(p1_old.getX(), p1_new.getX());
    ASSERT_EQ(p1_old.getV(), p1_new.getV());
    ASSERT_EQ(p1_old.getF(), p1_new.getF());
    ASSERT_EQ(p1_old.getOldF(), p1_new.getOldF());

    ASSERT_EQ(p2_old.getX(), p2_new.getX());
    ASSERT_EQ(p2_old.getV(), p2_new.getV());
    ASSERT_EQ(p2_old.getF(), p2_new.getF());
    ASSERT_EQ(p2_old.getOldF(), p2_new.getOldF());

    ASSERT_EQ(p3_old.getX(), p3_new.getX());
    ASSERT_EQ(p3_old.getV(), p3_new.getV());
    ASSERT_EQ(p3_old.getF(), p3_new.getF());
    ASSERT_EQ(p3_old.getOldF(), p3_new.getOldF());
}