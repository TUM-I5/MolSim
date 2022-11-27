#include <gtest/gtest.h>
#include "data/ParticleGenerator.h"
#include "data/Particle.h"
#include "io/BodyReader.h"

#include "io/Logging.h"

#include <list>
#include <Eigen>

TEST(BodyReader, readFile){
    io::BodyReader bodyReader;
    std::list<Particle> buffer;
    double eps{0.0};
    double sig{0.0};
    bodyReader.readFile("../input/bodyReaderTestFile.txt", buffer, eps, sig);

    std::vector<Eigen::Vector3d> supposed2BeThere = {{0.,0.,0.}, {0.,1.,0.}, 
        {1.,1.,1.}, {2.,1.,1.}, {1.,2.,1.}, {1.,1.,2.}, {4.,5.,6.},
        {3.2,24.0,42.5},
        {7.,7.,7.}};

	for(Eigen::Vector3d comparator : supposed2BeThere){
		//std::list<Particle>::iterator not_end = find_if(buffer.begin(), buffer.end(), [&](Particle p){return p.getX() == comparator;});
		std::list<Particle>::iterator not_end = find_if(buffer.begin(), buffer.end(), [&](Particle p){return p.getX()[0] == comparator[0] 
																					&& p.getX()[1] == comparator[1] && p.getX()[2] == comparator[2];});
		EXPECT_NE(not_end, buffer.end())<< "BodyReader did not invoke the ParticleGenerator properly";
	}
    EXPECT_EQ(eps, 0.1)<< "Eps not parsed in correctly";
    EXPECT_EQ(sig, 0.2)<< "Sigma not parsed correctly";
}