#include <gtest/gtest.h>
#include "particleModel/storage/CellContainer.h"
#include "particleModel/storage/CellContainerIterators.h"

#include "particleModel/updating/CellCalculator.h"




/**
 * @brief Validates reflective boundary conditions within the CellCalculator class.
 *        First a small CellContainer and a CellCalculator for that CellContainer
 *        are created. Everything is assumed to be in 2D. Then at every of the four 
 *        side boundaries (that is the boundaries in negative x, negative y, positive x and positive y direction)
 *        a particle is added as well as on particle in the middle. The forces of 
 *        the particles are initalized to be zero by their constructor. 
 *        Then boundary conditions are applied once and it is check 
 *        wether the force has changed, which should be the case for all particle except 
 *        for the one in the middle.
 */
TEST(test_BoundaryConditions,test_reflective){
    CellContainer cellContainer(4,4,1,1.0,1.0);

    CellCalculator cellCalculator(cellContainer,0.0014,
            {boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective},0);



    //adding Partciles in every relevant region
    cellContainer.addParticle({0.1,0.1,0},{0,0,0},1); //corner
    cellContainer.addParticle({2,0.1,0},{0,0,0},1); //x-border in negative direction in the middle 
    cellContainer.addParticle({2,3.99,0},{0,0,0},1); //x-border in positive direction in the middle 
    cellContainer.addParticle({3.99,2,0},{0,0,0},1); //y-border in positive direction in the middle 
    cellContainer.addParticle({0.1,2,0},{0,0,0},1); //y-border in negative direction in the middle

    cellContainer.addParticle({2,2,0},{0,0,0},1); //particle not at the boundary (further away then 1.1225 from boundary)
    //all of their forces will be initalized to 0

    cellContainer.createPointers();

    std::cout << cellContainer.to_string() << std::endl;

    cellCalculator.applyReflectiveBoundaries(); //now Boundary Conditions should have been applied to all particles

    std::cout << cellContainer.to_string() << std::endl;
    

    //iterate through "all" 5 particles and check, that at least some force was applied to them
    for(auto iter = cellContainer.begin(); iter != cellContainer.end(); ++iter){
        for(Particle* particle_ptr : *iter){
            std::cout << "Checking that force was applied to particle: " << std::endl;
            std::cout << particle_ptr->toString() << std::endl;
            //check that the force is not zero anymore
            if(particle_ptr->getX() == std::array<double,3>({2,2,0})){
                ASSERT_TRUE(particle_ptr->getF()[0] == 0 && particle_ptr->getF()[1] == 0 && particle_ptr->getF()[0] == 0);
            }else{
                ASSERT_TRUE(particle_ptr->getF()[0] != 0 || particle_ptr->getF()[1] != 0 || particle_ptr->getF()[2] != 0);
            }
        }
    }

}


/**
 * @brief Validates reflective outflow conditions within the CellCalculator class.
 *        First a small CellContainer and a CellCalculator for that CellContainer
 *        are created. Everything is assumed to be in 2D. Then at every of the four 
 *        side boundaries (that is the boundaries in negative x, negative y, positive x and positive y direction)
 *        a particle is added. Every particle has a relatively high velocity in the direction
 *        of his repsective boundary. Then some iterations are applied to see the change of positions 
 *        In the end check if all particles are gone, which should have happened because there is 
 *        border / restriction at the boundary in this case.
 */
TEST(test_BoundaryConditions,test_outflow){
    CellContainer cellContainer(4,4,1,1.0,1.0);

    //have a relatively big step size so change is visible already after a few iterations
    CellCalculator cellCalculator(cellContainer,0.005,
            {boundary_conditions::outflow,boundary_conditions::outflow,
        boundary_conditions::outflow,boundary_conditions::outflow,
        boundary_conditions::outflow,boundary_conditions::outflow},0);


    //same setup as in last test to inspect the relevant boundaries
    //but particles have high velocities in the direction of their
    //respective borders to see change relatively fast

    //adding Partciles in every relevant region
    cellContainer.addParticle({0.1,0.1,0},{-10,0,0},1); //corner
    cellContainer.addParticle({2,0.1,0},{0,-10,0},1); //x-border in negative direction in the middle 
    cellContainer.addParticle({2,3.99,0},{0,10,0},1); //x-border in positive direction in the middle 
    cellContainer.addParticle({3.99,2,0},{10,0,0},1); //y-border in positive direction in the middle 
    cellContainer.addParticle({0.1,2,0},{0,-10,0},1); //y-border in negative direction in the middle

    cellContainer.createPointers();

    std::cout << cellContainer.to_string() << std::endl;


    //doing some iterations to see change:
    for(int i = 0; i < 100; i++){ 
        cellCalculator.applyReflectiveBoundaries(); 
        cellCalculator.calculateLinkedCellF();
        cellCalculator.calculateWithinFVX();
    }
    

    std::cout << cellContainer.to_string() << std::endl;
    
    //all particles should be gone
    ASSERT_EQ(cellContainer.size(),0);

}