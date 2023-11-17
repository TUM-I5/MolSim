#include <gtest/gtest.h>
#include <gmock/gmock.h>
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
class MockParticleContainer: public ParticleContainer{
public:
    MOCK_METHOD(Particle*, getNextParticle, (), (override));
    //MOCK_METHOD(void, setNextPair, ((std::pair<Particle*,Particle*>)),(override));
};
using testing::Return;
TEST(simulation,test_runSimulation){
    MockParticleContainer mockParticleContainer;
    mockParticleContainer.addParticle({0,0,0},{0,1,0},1); //p1
    mockParticleContainer.addParticle({0,1,0},{0,-1,0},1);//p2
    mockParticleContainer.addParticle({0,0,1},{0,0,-1},1);//p3

    runSimulation(mockParticleContainer, 0.0000, 0.0002, 1);
    EXPECT_CALL(mockParticleContainer,getNextParticle()).Times(0);

 /*

     */
}