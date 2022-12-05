#include "data/ParticleGenerator.h"
#include "data/ParticleContainer.h"
#include "data/Particle.h"
#include "data/Body.h"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <Eigen>

void checkSupposed2BeThere(std::vector<Eigen::Vector3d>& supposed2BeThere, std::list<Particle>& buffer){
	for(Eigen::Vector3d& comparator : supposed2BeThere){
		//std::list<Particle>::iterator not_end = find_if(buffer.begin(), buffer.end(), [&](Particle p){return p.getX() == comparator;});
		std::list<Particle>::iterator not_end = find_if(buffer.begin(), buffer.end(), [&](Particle p){return p.getX()[0] == comparator[0] 
																					&& p.getX()[1] == comparator[1] && p.getX()[2] == comparator[2];});
		EXPECT_NE(not_end, buffer.end())<< "No Particle has been generated at ( " << comparator[0] << ", " << comparator[1] << ", " << comparator[2] << ")\n";
	}
}

void checkNotSupposed2BeThere(std::vector<Eigen::Vector3d>& notSupposed2BeThere, std::list<Particle>& buffer){
	for(Eigen::Vector3d& comparator : notSupposed2BeThere){
		std::list<Particle>::iterator shouldBeEnd = find_if(buffer.begin(), buffer.end(), [&](Particle p){return p.getX() == comparator;});
		EXPECT_EQ(shouldBeEnd, buffer.end())<< "Particle at ( " << comparator[0] << ", " << comparator[1] << ", " << comparator[2] << ") should not have been generated\n";
	}
}

/**
 * @brief Test if a Cuboid is generated with the desired properties
 * 
 */
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
	
	ParticleGenerator::generateCuboid(body, 0.1, buffer, 3);
	ASSERT_EQ(buffer.size(), 4*5*6) << "generateCuboid does not generate the right amount of Particles";
	std::vector<Eigen::Vector3d> supposed2BeThere = {{1., 1., 1.}, {2., 1., 1.}, {1., 2., 1.},  {1., 1., 2.}, {4.,5.,6.}};  
	checkSupposed2BeThere(supposed2BeThere, buffer);

	std::vector<Eigen::Vector3d> notSupposed2BeThere = {{0.,0.,0.}, {5.,5.,6.},{4.,6.,6.}, {4.,5.,7.}, {5.,6.,7.}}; 
	checkNotSupposed2BeThere(notSupposed2BeThere, buffer);
}

/**
 * @brief Test if the Sphere Generation works as intended
 * 
 */
TEST(ParticleGenerator, generateSphere)	{
	//test 3d generation:
	std::list<Particle> buffer{};

	struct Body body;
	body.shape = sphere;
	body.fixpoint  = {4., 4., 4.};
	body.dimensions= {3, 3, 3};
	body.distance = 1.0;
	body.mass = 1e-9;
	body.start_velocity = {0.,0.,0.};

	ParticleGenerator::generateSphere(body, 0.1, buffer, 3);

	std::vector<Eigen::Vector3d> supposed2BeThere{{4.,4.,4.}};
	//fill supposed2BeThere
	//straight lines
	for(double i = 1.; i <=3 ; i++){
		std::vector<Eigen::Vector3d> merger{{4.+i,4.,4.}, {4.-i,4.,4.}, {4.,4.+i,4.}, {4.,4.-i,4.}};
		supposed2BeThere.insert(supposed2BeThere.end(),
		std::make_move_iterator(merger.begin()),
		std::make_move_iterator(merger.end()));
	}
	//edgecases that should just be in range
	for(double i = 1.; i <=2 ; i++){
		std::vector<Eigen::Vector3d> merger{{4.+i,6.,5.}, {4.-i,4.-2.,5.}, {5.,4.+i,6.}, {4.-1.,4.-i,4.-2.}};
		supposed2BeThere.insert(supposed2BeThere.end(),
		std::make_move_iterator(merger.begin()),
		std::make_move_iterator(merger.end()));
	}

	checkSupposed2BeThere(supposed2BeThere, buffer);

	// 3 points are representative of things that could "just barely go wrong"
	std::vector<Eigen::Vector3d> notSupposed2BeThere{{1.,1.,1.}, {2.,2.,2.}, {8.,4.,4.}};
	checkNotSupposed2BeThere(notSupposed2BeThere, buffer);

	supposed2BeThere.clear();
	notSupposed2BeThere.clear();
	buffer.clear();
	ASSERT_EQ(0, buffer.size())<<"Buffer clearance did not work properly\n";
	ASSERT_EQ(0, supposed2BeThere.size())<<"Buffer clearance did not work properly\n";
	ASSERT_EQ(0, notSupposed2BeThere.size())<<"Buffer clearance did not work properly\n";

	//test 2d generation:


	struct Body body2d;
	body2d.shape = sphere;
	body2d.fixpoint  = {7.,7.,0.};
	body2d.dimensions= {4, 4, 4};
	body2d.distance = 1.0;
	body2d.mass = 1e-9;
	body2d.start_velocity = {0.,0.,0.};

	ParticleGenerator::generateSphere(body2d, 0.1, buffer, 2);
	
	supposed2BeThere.emplace_back(7.,7.,0.);
	supposed2BeThere.emplace_back(3.,7.,0.);
	supposed2BeThere.emplace_back(7.,11.,0.);
	supposed2BeThere.emplace_back(5.,4.,0.);
	
	checkSupposed2BeThere(supposed2BeThere, buffer);

	notSupposed2BeThere.emplace_back(7.,7.,1.);
	notSupposed2BeThere.emplace_back(6.,7.,-1.);
	notSupposed2BeThere.emplace_back(7.,12.,0);

	checkNotSupposed2BeThere(notSupposed2BeThere, buffer);

	//check 2-dimensional heat
	std::for_each(buffer.begin(), buffer.end(), [](Particle& p){
		ASSERT_EQ(0., p.getV()[2])<<"2-dimensional sphere got initiated with 3-dimensional heat!";
	});
}