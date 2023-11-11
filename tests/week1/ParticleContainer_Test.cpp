#include <gtest/gtest.h>
#include "particleModel/ParticleContainer.h"


/**
 * Test if the iterator works correctly by inserting three elements
 * and checking if the elements are returned in the correct order
*/
TEST(test_iterator,test_iterator1){
    ParticleContainer particles;
    particles.addParticle({0.0,0.0,0.0},{0.0,0.0,0.0},1.0);
    particles.addParticle({0.0,0.0,0.0},{0.0,0.0,0.0},1.0);
    particles.addParticle({0.0,0.0,0.0},{0.0,0.0,0.0},1.0);
    auto returned_p1 = particles.getNextParticle();
    EXPECT_TRUE(returned_p1 == &particles[0]);
    returned_p1 =  particles.getNextParticle();
    EXPECT_TRUE(returned_p1 == &particles[1]);
    returned_p1 =  particles.getNextParticle();
    EXPECT_TRUE(returned_p1 == &particles[2]);
}

