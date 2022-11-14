#include "data/ParticleGenerator.h"
#include "data/ParticleContainer.h"
#include "data/Particle.h"
#include "data/Body.h"

#include <vector>
#include <algorithm>
#include <Eigen>

/**
 * basically 
 */
bool isEqualPosition(const Particle & lhs, const Particle& rhs) {
    return (x == other.x);
}

TEST(ParticleGenerator, generateCuboid)	{
	//ParticleContainer particleContainer;
	std::list<Particle> buffer{};
	
	Body::struct Body body;
	body.shape = cuboid;
	body.fixpoint{1.,1.,1.};
	body.dimensions{4,5,6};
	body.distance = 1.0;	//there should be no rounding going on with this distance
	body.mass = 1e-9;
	body.start_velocity{2.,3.,4.};
	
	ParticleGenerator::generateCuboid(body, 0.1, buffer);
	ASSERT_EQ(buffer.size(), 4*5*6);
	
	Particle(const Eigen::Vector3d& x_arg, const Eigen::Vector3d& v_arg, double m_arg, int type = 0);
	//Particle::Particle comparator = Particle({1.,1.,1.}, {0.,0.,0}, 1e-9, 0);
	Eigen::Vector3d comparator{1.,1.,1.};
	
	std::list<Particle>::iterator not_end = find_if(buffer.begin(), buffer.end(), [&](Particle p){p.x == comparator});
	
	EXPECT_NEQ(not_end, buffer.end())<< "A particle that should be generated has not been generated!";
}