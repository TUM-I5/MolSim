#include <gtest/gtest.h>
#include "data/Particle.h"
#include "io/input/sim_input/BodyReader.h"
#include "io/input/arg_names.h"

#include <list>
#include <Eigen>

TEST(BodyReader, readFile){
    io::input::BodyReader bodyReader;
    std::list<Particle> buffer;
    double eps{0.0};
    double sig{0.0};
    std::unordered_map<io::input::names, std::string> argMap;
    bodyReader.readFile("../../input/bodyReaderTestFile.txt", buffer, argMap);

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

    eps = std::stod(argMap.at(io::input::names::epsilon));
    sig = std::stod(argMap.at(io::input::names::sigma));

    EXPECT_EQ(eps, 0.1)<< "Eps not parsed in correctly";
    EXPECT_EQ(sig, 0.2)<< "Sigma not parsed correctly";
}