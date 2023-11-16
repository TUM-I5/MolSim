#include <gtest/gtest.h>
#include "Simulation.h"
#include "particleModel/ParticleContainer.h"
#include "particleModel/Model.h"
#include "utils/ForceCalculations.h"
#include <iostream>

/**
 * @brief   First a ParticleContainer is created with three Particles
 *          and then the Simulation is intialized with this Container.
 *          Give the endtime and timegap, we see how many times of
 *          calculation is called during the iteration period, in the
 *          order of calculateF, calculateX and calculateV
 *
 *          At the beginning a Container with x particles do x(x-1)/2
 *          ForceCalculation because of the triangle algorithm, and
 *          then each iteration does x(x-1)/2 ForceCalculation, x
 *          PositionCalculation and x VelocityCalculation
 *
 *
*/

TEST(simulation,test_runSimulation){
    ParticleContainer particleContainer;
    particleContainer.addParticle({0,0,0},{0,1,0},1); //p1
    particleContainer.addParticle({0,1,0},{0,-1,0},1);//p2
    particleContainer.addParticle({0,0,1},{0,0,-1},1);//p3
    std::array<int,3> resultNoIteration = runSimulation(particleContainer, 0.0000, 0.0002, 1);
    ASSERT_EQ(resultNoIteration[0],3);
    ASSERT_EQ(resultNoIteration[1],0);
    ASSERT_EQ(resultNoIteration[2],0);
    std::array<int,3> resultSingleIteration = runSimulation(particleContainer, 0.0001, 0.0002, 1);
    ASSERT_EQ(resultSingleIteration[0],6);
    ASSERT_EQ(resultSingleIteration[1],3);
    ASSERT_EQ(resultSingleIteration[2],3);
    std::array<int,3> resultAlsoSingleIteration = runSimulation(particleContainer, 0.0002, 0.0002, 1);
    ASSERT_EQ(resultAlsoSingleIteration[0],6);
    ASSERT_EQ(resultAlsoSingleIteration[1],3);
    ASSERT_EQ(resultAlsoSingleIteration[2],3);
    std::array<int,3> resultTwoIterations = runSimulation(particleContainer, 0.0003, 0.0002, 1);
    ASSERT_EQ(resultTwoIterations[0],9);
    ASSERT_EQ(resultTwoIterations[1],6);
    ASSERT_EQ(resultTwoIterations[2],6);
    std::array<int,3> resultThreeIterations = runSimulation(particleContainer, 0.0005, 0.0002, 1);
    ASSERT_EQ(resultThreeIterations[0],12);
    ASSERT_EQ(resultThreeIterations[1],9);
    ASSERT_EQ(resultThreeIterations[2],9);

    particleContainer.addParticle({1,0,0},{1,0,0},1);//p4
    std::array<int,3> resultFourParticlesNoIteration = runSimulation(particleContainer, 0.0000, 0.0002, 1);
    ASSERT_EQ(resultFourParticlesNoIteration[0],6);
    ASSERT_EQ(resultFourParticlesNoIteration[1],0);
    ASSERT_EQ(resultFourParticlesNoIteration[2],0);
    std::array<int,3> resultFourParticlesSingleIteration = runSimulation(particleContainer, 0.0001, 0.0002, 1);
    ASSERT_EQ(resultFourParticlesSingleIteration[0],12);
    ASSERT_EQ(resultFourParticlesSingleIteration[1],4);
    ASSERT_EQ(resultFourParticlesSingleIteration[2],4);
    std::array<int,3> resultFourParticlesTwoIterations = runSimulation(particleContainer, 0.0003, 0.0002, 1);
    ASSERT_EQ(resultFourParticlesTwoIterations[0],18);
    ASSERT_EQ(resultFourParticlesTwoIterations[1],8);
    ASSERT_EQ(resultFourParticlesTwoIterations[2],8);
}