#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/model/ParticleContainer.h"
#include "../src/model/LinkedCellParticleContainer.h"
#include "../src/utils/ArrayUtils.h"

/**
 * test correct initialization of cells (especially boundary conditions)
 */
TEST(LinkedCellParticleContainer, Initialization)
{

    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9, 9, 9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    // 3 cells in each dimension, the 3 cells on the left/bottom should have a reflecting boundary on the left/bottom
    std::vector<ParticleCell> &cells = pc.getCells();
    EXPECT_EQ(cells.size(), 125);

    for (unsigned int i = 0; i < cells.size(); i++)
    {
        //halo cells
        if (i < 25 || i >= 100 || i % 5 == 0 || i % 5 == 4 || (i/5) % 5 == 0 || (i/5) % 5 == 4) {
            EXPECT_EQ(cells[i].getType(), CellType::HaloCell);
            EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(out, out, out, out, out, out));
        }
        // only one inner cell
        else if (i == 62)
        {
            EXPECT_EQ(cells[i].getType(), CellType::InnerCell);
            EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(out, out, out, out, out, out));
        }
        else
        {
            EXPECT_EQ(cells[i].getType(), CellType::BoundaryCell);
            // cell on the left bottom edge
            if (i == 31 || i == 56 || i == 81)
            {
                EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out));
            }
            // cell on the left side
            else if (i == 36 || i == 41 || i == 61 || i == 66 || i == 86 || i == 91)
            {
                EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(BoundaryCondition::Reflecting, out, out, out, out, out));
            }
            // cell on the bottom side
            else if (i == 32 || i == 33 || i == 57 || i == 58 || i == 82 || i == 83)
            {
                EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(out, out, BoundaryCondition::Reflecting, out, out, out));
            }
            // cell somewhere else
            else
            {
                EXPECT_THAT(cells[i].getBoundaries(), testing::ElementsAre(out, out, out, out, out, out));
            }
        }
    }
}

/**
 * test correct application of function to all particles & their position changes
 * by comparison to hand-calculated new positions
 */
TEST(LinkedCellParticleContainer, IterateParticles)
{
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9, 9, 9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    std::array<double, 3> x1 = {0, 0, 0};   // this should stay in the same cell
    std::array<double, 3> x2 = {2.5, 0, 0}; // this should cross to its adjacent cell
    std::array<double, 3> x3 = {8.5, 0, 0}; // this should cross to the halo
    std::array<double, 3> addX = {1, 0, 0};
    std::array<double, 3> v = {0, 0, 0};
    double m = 1;
    double epsilon = 1; 
    double sigma = 1; 

    pc.reserveMemoryForParticles(3);
    pc.addParticle(x1, v, m, epsilon, sigma);
    pc.addParticle(x2, v, m, epsilon, sigma);
    pc.addParticle(x3, v, m, epsilon, sigma);

    pc.iterateParticles([addX](Particle &p)
                        { p.setX(p.getX() + addX); });

    EXPECT_EQ(pc.getActiveParticles().size(), 2);
    EXPECT_EQ(pc.getHaloParticles().size(), 1);

    std::vector<ParticleCell> &cells = pc.getCells();
    EXPECT_EQ(cells[31].size(), 1);
    EXPECT_EQ(cells[32].size(), 1);
}

/**
 * test correct application of linked cell algorithm against hand calculated values
 */
TEST(LinkedCellParticleContainer, IterateParticleInteractions)
{
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9, 9, 9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, out, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    std::array<double, 3> x1 = {0.5, 0, 0}; // only interaction with border
    std::array<double, 3> x2 = {4, 2.5, 0}; // only interactinon with x3
    std::array<double, 3> x3 = {4, 3.5, 0}; // only interaction with x2
    std::array<double, 3> x4 = {8, 8, 2.5}; // only interaction with x5
    std::array<double, 3> x5 = {8, 8, 3.5}; // only interaction with x6

    std::array<double, 3> v = {0, 0, 0};
    double m = 1;
    double epsilon = 1; 
    double sigma = 1; 

    pc.reserveMemoryForParticles(5);
    pc.addParticle(x1, v, m, epsilon, sigma);
    pc.addParticle(x2, v, m, epsilon, sigma);
    pc.addParticle(x3, v, m, epsilon, sigma);
    pc.addParticle(x4, v, m, epsilon, sigma);
    pc.addParticle(x5, v, m, epsilon, sigma);

    // force calculation function
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

    EXPECT_THAT(particles[0].getF(), testing::ElementsAre(120, 0, 0));
    EXPECT_THAT(particles[1].getF(), testing::ElementsAre(0, -120, 0));
    EXPECT_THAT(particles[2].getF(), testing::ElementsAre(0, 120, 0));
    EXPECT_THAT(particles[3].getF(), testing::ElementsAre(0, 0, -120));
    EXPECT_THAT(particles[4].getF(), testing::ElementsAre(0, 0, 120));
}

/**
 * test correct boundary force application by comparison to hand-calculated forces
 */
TEST(LinkedCellParticleContainer, ReflectingBoundaryCondition)
{
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9, 9, 9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Reflecting, out, BoundaryCondition::Reflecting, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    // force calculation function
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

    std::array<double, 3> x1 = {0.5, 0.5, 0}; // reflection force from both borders
    std::array<double, 3> x2 = {0.5, 1, 0};   // reflection force from left border
    std::array<double, 3> x3 = {1, 0.5, 0};   // reflection force from bottom border
    std::array<double, 3> x4 = {2, 2, 0};     // no reflection force
    std::array<double, 3> v = {0, 0, 0};
    double m = 1;
    double epsilon = 1; 
    double sigma = 1; 
    Particle p1 = Particle(x1, v, m, epsilon, sigma);
    Particle p2 = Particle(x2, v, m, epsilon, sigma);
    Particle p3 = Particle(x3, v, m, epsilon, sigma);
    Particle p4 = Particle(x4, v, m, epsilon, sigma);
    std::vector<Particle *> dummyCell;
    dummyCell.reserve(4);
    dummyCell.push_back(&p1);
    dummyCell.push_back(&p2);
    dummyCell.push_back(&p3);
    dummyCell.push_back(&p4);

    pc.reflectingBoundary(dummyCell, 0, forceCalculationIteration);
    EXPECT_THAT(p1.getF(), testing::ElementsAre(120, 0, 0));
    EXPECT_THAT(p2.getF(), testing::ElementsAre(120, 0, 0));
    EXPECT_THAT(p3.getF(), testing::ElementsAre(0, 0, 0));
    EXPECT_THAT(p4.getF(), testing::ElementsAre(0, 0, 0));

    pc.reflectingBoundary(dummyCell, 2, forceCalculationIteration);
    EXPECT_THAT(p1.getF(), testing::ElementsAre(120, 120, 0));
    EXPECT_THAT(p2.getF(), testing::ElementsAre(120, 0, 0));
    EXPECT_THAT(p3.getF(), testing::ElementsAre(0, 120, 0));
    EXPECT_THAT(p4.getF(), testing::ElementsAre(0, 0, 0));
}

/**
 * test correct transitions to halo since only active particles are considered for calculations & output
 * therefore particles in halo can be seen as "removed" from the simulation
 */
TEST(LinkedCellParticleContainer, OutflowBoundaryCondition) {
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9,9,9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {out, out, out, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    std::array<double, 3> x1 = {0, 0, 0}; //particle should cross to halo 
    std::array<double, 3> v1 = {-1,0,0}; 

    std::array<double,3> x2 = {5, 8, 0}; //particle should cross to halo
    std::array<double, 3> v2 = {2, 3, 0} ;

    std::array<double,3> x3 = {3, 3, 8}; //particle should cross to halo
    std::array<double, 3> v3 = {0, 0, 2}; 

    std::array<double, 3> x4 = {4, 4, 4}; //particle should stay where it is
    std::array<double, 3> v4 = {0, 0, 0};

    double m = 1;
    double epsilon = 1; 
    double sigma = 1; 
    
    //calcX with delta_t = 1, f is initialized to zero
    std::function<void(Particle &)> f = [delta_t = 1](Particle &p1)
    {
        std::array<double, 3> x_new = p1.getX() + delta_t * p1.getV() + (delta_t * delta_t / (2 * p1.getM())) * p1.getF();
        p1.setX(x_new);
    };

    pc.reserveMemoryForParticles(4);
    pc.addParticle(x1, v1, m, epsilon, sigma);
    pc.addParticle(x2, v2, m, epsilon, sigma);
    pc.addParticle(x3, v3, m, epsilon, sigma);
    pc.addParticle(x4, v4, m, epsilon, sigma);

    EXPECT_EQ(pc.getActiveParticles().size(), 4);

    pc.iterateParticles(f);

    EXPECT_EQ(pc.getActiveParticles().size(), 1);
    EXPECT_EQ(pc.getHaloParticles().size(), 3);
}

/**
 * Test correct force application over opposite domain boundaries against hand-calculated values
*/
TEST(PeriodicBoundaryCondition, ForceApplication) {
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9,9,9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Periodic, BoundaryCondition::Periodic, out, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    std::array<double, 3> x1 = {0.5, 0.5, 0};
    std::array<double, 3> x2 = {8.5, 0.5, 0};
    std::array<double, 3> v = {0, 0, 0};
    double m = 1;
    double sigma = 1;
    double epsilon = 5;

    // force calculation function
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
        //std::cout << "F_ij: " << f_ij << std::endl;
        //std::cout << "F_ji: " << f_ji << std::endl;

        p1.addF(f_ij);
        p2.addF(f_ji);
    };

    pc.reserveMemoryForParticles(2);
    pc.addParticle(x1, v, m, epsilon, sigma);
    pc.addParticle(x2, v, m, epsilon, sigma);

    pc.iterateParticleInteractions(forceCalculationIteration);
    std::vector<Particle> &particles = pc.getActiveParticles();

    EXPECT_EQ(particles.size(), 2);
    EXPECT_THAT(particles[0].getF(), testing::ElementsAre(120, 0, 0));
    EXPECT_THAT(particles[1].getF(), testing::ElementsAre(-120, 0, 0));
}

/**
 * Test correct reappearance of particle on other domain side
*/
TEST(PeriodicBoundaryCondition, CrossSides) {
    double reflectingDistance = 0.5;
    double cutoff = 3;
    std::array<double, 3> domain = {9,9,9};
    BoundaryCondition out = BoundaryCondition::Outflow;
    std::array<BoundaryCondition, 6> boundaries = {BoundaryCondition::Periodic, BoundaryCondition::Periodic, out, out, out, out};
    LinkedCellParticleContainer pc = LinkedCellParticleContainer(reflectingDistance, cutoff, domain, boundaries);

    std::array<double, 3> x = {0.5, 0.5, 0};
    std::array<double, 3> v = {-1, 0, 0};
    double m = 1;
    double sigma = 1;
    double epsilon = 5;

    //calcX with delta_t = 1, f is initialized to zero
    std::function<void(Particle &)> f = [delta_t = 1](Particle &p1)
    {
        std::array<double, 3> x_new = p1.getX() + delta_t * p1.getV() + (delta_t * delta_t / (2 * p1.getM())) * p1.getF();
        p1.setX(x_new);
    };

    pc.reserveMemoryForParticles(1);
    pc.addParticle(x, v, m, sigma, epsilon);
    pc.iterateParticles(f);
    std::vector<Particle> &particles = pc.getActiveParticles();

    EXPECT_THAT(particles[0].getX(), testing::ElementsAre(8.5, 0.5, 0));
}