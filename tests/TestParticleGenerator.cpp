#include "../src/data/ParticleGenerator.h"
#include "../src/data/ParticleContainer.h"
#include "../src/data/Particle.h"
#include "../src/data/Body.h"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <Eigen>

TEST(ParticleGenerator, generateCuboid)	{
	//ParticleContainer particleContainer;
	std::list<Particle> buffer{};
	
	struct Body body;
	body.shape = cuboid;
	body.fixpoint  = {1., 1., 1.};
	body.dimensions= {4, 5, 6};
	body.distance = 1.0;	//there should be no rounding going on with this distance
	body.mass = 1e-9;
	body.start_velocity = {2.,3.,4.};
	
	ParticleGenerator::generateCuboid(body, 0.1, buffer);
	ASSERT_EQ(buffer.size(), 4*5*6) << "generateCuboid does not generate the right amount of Particles";
	
	//Particle::Particle comparator = Particle({1.,1.,1.}, {0.,0.,0}, 1e-9, 0);
	//std::vector<Eigen::Vector3d> supposed2BeThere = {{1., 1., 1.}, {2., 1., 1.}, {1., 2., 1.}, {1., 1., 2.}, {5.,6.,7.}};  

	std::vector<Eigen::Vector3d> supposed2BeThere = {{1., 1., 1.}, {2., 1., 1.}, {1., 2., 1.},  {1., 1., 2.}, {4.,5.,6.}};  

	
	for(Eigen::Vector3d comparator : supposed2BeThere){
		//std::list<Particle>::iterator not_end = find_if(buffer.begin(), buffer.end(), [&](Particle p){return p.getX() == comparator;});
		std::list<Particle>::iterator not_end = find_if(buffer.begin(), buffer.end(), [&](Particle p){return p.getX()[0] == comparator[0] 
																					&& p.getX()[1] == comparator[1] && p.getX()[2] == comparator[2];});
		EXPECT_NE(not_end, buffer.end())<< "A particle that should be generated has not been generated!";
	}
	
	std::vector<Eigen::Vector3d> notSupposed2BeThere = {{0.,0.,0.}, {5.,5.,6.},{4.,6.,6.}, {4.,5.,7.}, {5.,6.,7.}}; 
	for(Eigen::Vector3d comparator : notSupposed2BeThere){
		std::list<Particle>::iterator shouldBeEnd = find_if(buffer.begin(), buffer.end(), [&](Particle p){return p.getX() == comparator;});
		EXPECT_EQ(shouldBeEnd, buffer.end())<< "A particle that should not have been generated has been generated!";
	}
}
