/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "BodyReader_secondAttempt.h"
#include "ParticleGenerator.h"
#include "Body.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>


namespace io {
    BodyReader::BodyReader() = default;

    BodyReader::~BodyReader() = default;

    void BodyReader::readFile(const char* filename, ParticleGenerator& particleGenerator) {
        std::array<double, 3> x;
        std::array<double, 3> v;
        double m;
        int num_particles = 0;

        std::ifstream input_file(filename);
        std::string tmp_string;

        if (input_file.is_open()) {

            getline(input_file, tmp_string);
            std::cout << "Read line: " << tmp_string << std::endl;

            while (tmp_string.empty() or tmp_string[0] == '#') {
                getline(input_file, tmp_string);
                std::cout << "Read line: " << tmp_string << std::endl;
            }

            std::istringstream numstream(tmp_string);
            numstream >> num_particles;
            std::cout << "Reading " << num_particles << "." << std::endl;
            getline(input_file, tmp_string);
            std::cout << "Read line: " << tmp_string << std::endl;

            for (int i = 0; i < num_particles; i++) {
                std::istringstream datastream(tmp_string);

                for (auto &xj: x) {
                    datastream >> xj;
                }
                for (auto &vj: v) {
                    datastream >> vj;
                }
                if (datastream.eof()) {
                    std::cout
                            << "Error reading file: eof reached unexpectedly reading from line "
                            << i << std::endl;
                    exit(-1);
                }
                datastream >> m;

                //Shape extensions starting here
                if (datastream.eof()){
                    particleGenerator.generateParticle(x, v, m);
                }
                else{
                    struct Body body;
                    std::string shapeString;
                    datastream>>shapeString;
                    body.shape = shapeFromString(shapeString);
                    body.fixpoint << x[0], x[1], x[2];
                    body.start_velocity << v[0], v[1], v[2];
                    body.mass = m;
                    datastream >> body.distance;
                    datastream >> body.start_velocity[0];
                    datastream >> body.start_velocity[1];
                    datastream >> body.start_velocity[2];
                    double brown_average = 0.1;     
                    //TODO: initialize the constant values globally properly from input file
                    switch(body.shape){
                        case cuboid:
                            particleGenerator.generateCuboid(body, brown_average);
                            break;
                        case sphere:
                            std::cout<<"Body Sphere not implemented yet\n";
                    }
                }

                getline(input_file, tmp_string);
                std::cout << "Read line: " << tmp_string << std::endl;
            }
        } else {
            std::cout << "Error: could not open file " << filename << std::endl;
            exit(-1);
        }
    }

    enum Shape BodyReader::shapeFromString(std::string& shape){
        const std::array<std::string, number_of_shapes> strOfShape = {"Cuboid", "Sphere", "Particle"};
        for(size_t i = 0; i < number_of_shapes; i++){
            if(boost::iequals(shape, strOfShape[i])){
                return all_shapes[i];   //all shapes defined in Body.h
            } 
        }
        std::cerr<< "Couldn't interpret %s as shape",shape;
        exit(-1);
    }
} // io