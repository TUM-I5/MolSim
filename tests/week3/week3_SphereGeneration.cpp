#include <gtest/gtest.h>
#include "inputHandling/FileReader.h"
#include "inputHandling/SphereGeneration.h"
#include "particleModel/storage/ParticleContainer.h"

TEST (sphereGeneration,test_sphereGeneration){

    FileReader::SphereData s1{{0, 0, 0}, {0, 0, 0}, 1, 2, 1.1225, 1, 5};

    std::list<FileReader::SphereData> spheres{s1};

    ParticleContainer particleContainer;

    addSpheres(particleContainer, spheres, 2);

   // check that the z dimention of the spheres is set to 0

    for(int i = 0; i < particleContainer.size(); i++) {
    ASSERT_EQ(particleContainer[i].getV().at(2), 0.0);
    }

    // check that a sphere with the radius of 2 has 13 particles inside
    // as             *
    //             *  *  *
    //          *  *  *  *  *
    //             *  *  *
    //                *
    ASSERT_EQ(particleContainer.size(), 13);

}