#include <gtest/gtest.h>
#include "inputHandling/FileReader.h"
#include "inputHandling/CuboidGeneration.h"
#include "particleModel/ParticleContainer.h"


TEST(cuboidgeneration,test_cuboid_dimension){
    FileReader::CuboidData c1{{0, 0, 0}, {0, 0, 0}, 300, 5, 1, 1, 1.1225, 1, 5};
    FileReader::CuboidData c2{{0, 0, 0}, {0, 0, 0}, 1, 1, 1, 1, 1.1225, 1, 5};
    FileReader::CuboidData c3{{0, 0, 0}, {0, 0, 0}, 1, 1, 2, 1, 1.1225, 1, 5};
    FileReader::CuboidData c4{{0, 0, 1}, {0, 0, 0}, 1, 1, 1, 1, 1.1225, 1, 5};
    std::list<FileReader::CuboidData> cuboids{c1, c2};

    //check if velocity dimension is set to 2
    ParticleContainer particleContainer;
    addCuboids(particleContainer, cuboids);

    for(int i = 0; i < particleContainer.size(); i++) {
        ASSERT_EQ(particleContainer[i].getV().at(2), 0.0);
    }

    //check if velocity dimension is set to 3, as a result of increasing N3
    cuboids.push_back(c3);
    particleContainer = ParticleContainer();
    addCuboids(particleContainer, cuboids);

    bool correctDimA{false};
    for(int i = 0; i < particleContainer.size(); i++) {
        if(0.0 < particleContainer[i].getV().at(2)) {
            correctDimA = true;
            break;
        }
    }
    ASSERT_TRUE(correctDimA);

    //check if velocity dimension is set to 3, as a result of different Z coordinates
    cuboids.remove(c3);
    cuboids.push_back(c4);
    particleContainer = ParticleContainer();
    addCuboids(particleContainer, cuboids);

    bool correctDimB{false};
    for(int i = 0; i < particleContainer.size(); i++) {
        if(0.0 < particleContainer[i].getV().at(2)) {
            correctDimB = true;
            break;
        }
    }
    ASSERT_TRUE(correctDimB);
}

TEST(cuboidgeneration,test_cuboidgeneration){
    double h{1.1225};
    FileReader::CuboidData c1{{10, 10, 10}, {0, 0, 0}, 10, 5, 10, 1, h, 1, 5};
    FileReader::CuboidData c2{{0, 0, 0}, {0, 0, 0}, 10, 15, 20, 1, h, 1, 5};
    std::list<FileReader::CuboidData> cuboids{c1, c2};

    //check if first and last particles position from second cuboid got created correctly
    ParticleContainer particleContainer;
    addCuboids(particleContainer, cuboids);
    std::array<double, 3> x_f{0.0, 0.0, 0.0};
    std::array<double, 3> x_s{9 * h, 14 * h, 19 * h};

    bool found_first{false};
    bool found_second{false};
    for(int i = 0; i < particleContainer.size(); i++) {
        if(particleContainer[i].getX() == x_f) {
            found_first = true;
        } else if(particleContainer[i].getX() == x_s) {
            found_second = true;
        }
    }
    ASSERT_TRUE(found_first);
    ASSERT_TRUE(found_second);
}