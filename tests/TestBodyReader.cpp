#include <gtest/gtest.h>
#include "data/ParticleGenerator.h"
#include "data/Particle.h"
#include "io/BodyReader.h"

#include <list>
#include <Eigen>

void BodyReader::readFile(const char *filename, std::list<Particle> &buffer, double& eps, double& sig) {
char * input_file = "#
# reformed file format: 
# Lines of comment start with '#' and are allowed at the beginning of the file and after just before the definition of epsilon and sigma
# Empty lines are not allowed.
# The first line not being a comment has to be one integer, indicating the number of bodies incomnig
#
# Bodies consists of 
# * xyz-coordinates (3 double values)
# * velocities (2 or 3 double values)
# * mass (1 double value)
# * shape (cuboid, sphere or particle)
# * dimensions (3 int values)
# * distance (1 double value)
# where the use of shape, dimensions and distance is optional. not using them will result in one particle being created.
# You cannot e.g. define the shape cuboid without defining its dimensions or distance
#
# xyz-coord      velocity        mass       shape       dimensions      distance  
5
0.0 0.0 0.0      0.0 0.0 0.0     1.0
0.0 1.0 0.0     -1.0 0.0 0.0     3.0e-6
1.0 1.0 1.0      0.0 0.0 0.0     1.0e-9     cuboid      4   5   6         1.0
3.2 24.0 42.5    1.2 2.0 2.75    4.0e-12    PaRtIcLe    1   1   1         0.5
7.0 7.0 7.0      1.0 1.0 1.0     2.0e-8     CuboiD      1   1   1         3.0
# epsilon    sigma
0.1         0.2"

TEST(BodyReader, readFile){
    io::BodyReader bodyReader;
    std::list<Particle> buffer;
    double eps{0.0};
    double sig{0.0};
    bodyReader.readFile(input_file, buffer, eps, sig);

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