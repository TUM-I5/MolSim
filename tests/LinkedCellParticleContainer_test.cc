#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/ParticleContainer.h"
#include "../src/model/LinkedCellParticleContainer.h"
#include "../src/utils/ArrayUtils.h"


/**
 * test correct initialization of cells (especially boundary conditions)
*/
TEST(LinkedCellParticleContainer, Initialization) {
  
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9,9,9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);
    
    //3 cells in each dimension, the 3 cells on the left/bottom should have a reflecting boundary on the left/bottom
    std::vector<ParticleCell> &cells = pc.getCells();
    EXPECT_EQ(cells.size(), 27);

    for (unsigned int i = 0; i < cells.size(); i++) {
        //only one inner cell
        if (i == 13) {
            EXPECT_EQ(cells[i].getType(), CellType::InnerCell); 
            EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(out, out, out, out, out, out));
        }
        else {
            EXPECT_EQ(cells[i].getType(), CellType::BoundaryCell);
            //cell on the left bottom edge
            if (i % 9 == 0) {
                EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out));
            }
            //cell on the left side 
            else if (i % 3 == 0) {
                EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(BoundaryCondition::Reflecting, out, out, out, out, out));
            }
            //cell on the bottom side
            else if ((i / 3) % 3 == 0) {
                 EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(out, out, BoundaryCondition::Reflecting, out, out, out));
            }
            //cell somewhere else
            else {
                EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(out, out, out, out, out, out));
            }
        }
    }
}

/**
 * test correct application of function to all particles & their position changes 
 * by comparison to hand-calculated new positions
*/
TEST(LinkedCellParticleContainer, IterateParticles) {
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9,9,9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    std::array<double, 3> x1 = {0,0,0}; //this should stay in the same cell
    std::array<double, 3> x2 = {2.5,0,0}; //this should cross to its adjacent cell
    std::array<double, 3> x3 = {8.5,0,0}; //this should cross to the halo
    std::array<double, 3> addX = {1,0,0};
    std::array<double, 3> v = {0,0,0};
    double m = 1;
    
    pc.reserveMemoryForParticles(3);
    pc.addParticle(x1,v,m);
    pc.addParticle(x2,v,m);
    pc.addParticle(x3,v,m);

    pc.iterateParticles([addX](Particle &p) {p.setX(p.getX() + addX);});

    EXPECT_EQ(pc.getActiveParticles().size(), 2);
    EXPECT_EQ(pc.getHaloParticles().size(), 1);

    std::vector<ParticleCell> &cells = pc.getCells();
    EXPECT_EQ(cells[0].size(), 1);
    EXPECT_EQ(cells[1].size(), 1);

}

/**
 * test correct application of linked cell algorithm against hand calculated values
*/
TEST(LinkedCellParticleContainer, IterateParticleInteractions) {
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9,9,9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, out, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    std::array<double, 3> x1 = {0.5, 0, 0}; //only interaction with border
    std::array<double, 3> x2 = {4, 2.5, 0}; //only interactinon with x3
    std::array<double, 3> x3 = {4, 3.5, 0}; //only interaction with x2
    std::array<double, 3> x4 = {8, 8, 2.5}; //only interaction with x5
    std::array<double, 3> x5 = {8, 8, 3.5}; //only interaction with x6

    std::array<double, 3> v = {0,0,0};
    double m = 1;

    pc.reserveMemoryForParticles(5);
    pc.addParticle(x1,v,m);
    pc.addParticle(x2,v,m);
    pc.addParticle(x3,v,m);
    pc.addParticle(x4,v,m);
    pc.addParticle(x5,v,m);

     //force calculation function 
    std::function<void(Particle &, Particle &)> forceCalculationIteration = [](Particle &p1, Particle &p2)
    {
        double distance = ArrayUtils::L2Norm(p1.getX() - p2.getX());

        // Reduce number of operation by reusing previous results
        double pow1 = 1 / distance;
        double pow2 = pow1 * pow1;
        double pow4 = pow2 * pow2;
        double pow6 = pow4 * pow2;
        double pow12 = pow6 * pow6;

        // Lennard-Jones force
        std::array<double, 3> f_ij = (-24 * 5 / pow(distance, 2)) * (pow6 - 2 * pow12) * (p1.getX() - p2.getX());
        std::array<double, 3> f_ji = -1 * f_ij;

        p1.addF(f_ij);
        p2.addF(f_ji);
    };

    pc.iterateParticleInteractions(forceCalculationIteration);

    std::vector<Particle> &particles = pc.getActiveParticles();
    
    EXPECT_THAT(particles[0].getF(), testing::ElementsAre(120,0,0));
    EXPECT_THAT(particles[1].getF(), testing::ElementsAre(0,-120,0));
    EXPECT_THAT(particles[2].getF(), testing::ElementsAre(0,120,0));
    EXPECT_THAT(particles[3].getF(), testing::ElementsAre(0,0,-120));
    EXPECT_THAT(particles[4].getF(), testing::ElementsAre(0,0,120));

}

/**
 * test correct boundary force application by comparison to hand-calculated forces
*/
TEST(LinkedCellParticleContainer, ReflectingBoundaryCondition) {
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9,9,9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    //force calculation function 
    std::function<void(Particle &, Particle &)> forceCalculationIteration = [](Particle &p1, Particle &p2)
    {
        double distance = ArrayUtils::L2Norm(p1.getX() - p2.getX());

        // Reduce number of operation by reusing previous results
        double pow1 = 1 / distance;
        double pow2 = pow1 * pow1;
        double pow4 = pow2 * pow2;
        double pow6 = pow4 * pow2;
        double pow12 = pow6 * pow6;

        // Lennard-Jones force
        std::array<double, 3> f_ij = (-24 * 5 / pow(distance, 2)) * (pow6 - 2 * pow12) * (p1.getX() - p2.getX());
        std::array<double, 3> f_ji = -1 * f_ij;

        p1.addF(f_ij);
        p2.addF(f_ji);
    };


    std::array<double, 3> x1 = {0.5, 0.5, 0}; //reflection force from both borders
    std::array<double, 3> x2 = {0.5, 1, 0}; //reflection force from left border
    std::array<double, 3> x3 = {1, 0.5, 0}; //reflection force from bottom border
    std::array<double, 3> x4 = {2,2,0}; //no reflection force
    std::array<double, 3> v = {0,0,0};
    double m = 1;
    Particle p1 = Particle(x1,v,m);
    Particle p2 = Particle(x2,v,m);
    Particle p3 = Particle(x3,v,m);
    Particle p4 = Particle(x4,v,m);
    std::vector<Particle *> dummyCell;
    dummyCell.reserve(4);
    dummyCell.push_back(&p1);
    dummyCell.push_back(&p2);
    dummyCell.push_back(&p3);
    dummyCell.push_back(&p4);

    pc.reflectingBoundary(dummyCell, 0, forceCalculationIteration);
    EXPECT_THAT(p1.getF(), testing::ElementsAre(120,0,0));
    EXPECT_THAT(p2.getF(), testing::ElementsAre(120,0,0));
    EXPECT_THAT(p3.getF(), testing::ElementsAre(0,0,0));
    EXPECT_THAT(p4.getF(), testing::ElementsAre(0,0,0));

    pc.reflectingBoundary(dummyCell, 2, forceCalculationIteration);
    EXPECT_THAT(p1.getF(), testing::ElementsAre(120,120,0));
    EXPECT_THAT(p2.getF(), testing::ElementsAre(120,0,0));
    EXPECT_THAT(p3.getF(), testing::ElementsAre(0,120,0));
    EXPECT_THAT(p4.getF(), testing::ElementsAre(0,0,0));

}