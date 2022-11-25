#include <gtest/gtest.h>

#include "data/ParticleContainer.h"
#include "data/Particle.h"

#include <vector>

/**
 * @brief Helper function that initializes a tiny particleContainer for testing
 * 
 */
ParticleContainer initializeTestPContainer(){
    std::vector<Particle> particles{};
    double r_cutoff = 1.2;
    for(double x=0.; x < 3; x++){
        for(double y=0.; y<3;y++){
            for(double z=0.; z<3; z++){
                Eigen::Vector3d pos{r_cutoff*x + 0.1, r_cutoff*y + 0.1,r_cutoff*z + 0.1};
                Eigen::Vector3d vel{0.,0.,0.};
                particles.emplace_back(pos, vel, 1.0, 0);
            }
        }
    }
    return ParticleContainer(particles, {3.1, 3.1, 3.1}, r_cutoff);
}

/**
 * Test if the ParticleContainer initializes the Cell-Datastructure as intended 
 * (with the proper amount of cells, the right dimensions, the particles in the right cells, etc) 
 * 
 */
TEST(ParticleContainer, Cell_Initialization){
    ParticleContainer particleContainer = initializeTestPContainer();

    ASSERT_EQ(particleContainer.size(), 27) << "ParticleContainer did not get initialized with the right amount of items!";
    std::array<unsigned int, 3> expected_dimensions{3,3,3};
    ASSERT_EQ(particleContainer.getGridDimensions(), expected_dimensions) << "ParticleContainer did not initialize the grid with the right dimensions";

    //check if every cell actually has one element
    particleContainer.forAllCells([](std::vector<double> &, std::vector<double> &, std::vector<double> &, std::vector<double> &, 
        std::vector<double> &, std::vector<int> &, unsigned long , std::vector<size_t> &cellItems){
        ASSERT_EQ(cellItems.size(), 1)<<"ParticleContainer did not assign the right amount of Particles into each cell";    
    });
}