#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "particleModel/ParticleContainer.h"
#include "inputHandling/FileReader.h"




void writeCuboidIntoFile(const FileReader::CuboidData& cuboid,const char * filename ){
    std::ofstream outfile(filename,std::ios::app);

    if (outfile.is_open()) {
        outfile << std::fixed << std::setprecision(5);

        outfile << "cuboid:\n";
        outfile << "    position: {" << cuboid.x[0] << "," << cuboid.x[1] << "," << cuboid.x[2] << "}\n";
        outfile << "    velocity: {" << cuboid.v[0] << "," << cuboid.v[1] << "," << cuboid.v[2] << "}\n";
        outfile << "    (N1 x N2 x N3): {" << cuboid.N1 << "," << cuboid.N2 << "," << cuboid.N3 << "}\n";
        outfile << "    mass: " << cuboid.m << "\n";
        outfile << "    mesh-width: " << cuboid.h << "\n";
        outfile << "    sigma: " << cuboid.sigma << "\n";
        outfile << "    epsilon: " << cuboid.epsilon << "\n";

        outfile << '\n';
        outfile.close();
        std::cout << "Data written to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}






TEST(filereader,test_filereader_one_cuboid_in_file){


    FileReader fileReader;

    FileReader::CuboidData cuboid{{0, 0, 0}, {0, 0, 0}, 40, 8, 1, 1, 1.1225, 1, 5};

    const char* filename = "test.txt";
    
    writeCuboidIntoFile(cuboid,filename);

    auto cuboid_list = fileReader.readCuboidFile("test.txt");


    ASSERT_EQ(cuboid,cuboid_list.front());

}


TEST(filereader,test_filereader_several_cuboids_in_file){

    FileReader fileReader;


    FileReader::CuboidData cuboid{{0, 0, 0}, {0, 0, 0}, 40, 8, 1, 1, 1.1225, 1, 5};

    FileReader::CuboidData cuboid1{{0.0001, 7.56, 3.434}, {3034, 324, 25464}, 1000, 23, 11, 0.0001, 50505, 1, 3};

    FileReader::CuboidData cuboid2{{0.123, 456.789, 987.654}, {321.0, 654.321, 123.456}, 789012, 45, 67, 0.654, 1, 8, 13234};


    const char * filename = "test1.txt";
    
    writeCuboidIntoFile(cuboid,filename);
    writeCuboidIntoFile(cuboid1,filename);
    writeCuboidIntoFile(cuboid2,filename);

    auto cuboid_list = fileReader.readCuboidFile("test1.txt");


    ASSERT_EQ(cuboid,cuboid_list.front());
    cuboid_list.pop_front();
    ASSERT_EQ(cuboid1,cuboid_list.front());
    cuboid_list.pop_front();
    ASSERT_EQ(cuboid2,cuboid_list.front());
}
