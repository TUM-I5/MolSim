#pragma once

#include "data/Body.h"
#include "data/ParticleGenerator.h"

#include <list>
#include <vector>
#include <Eigen>

namespace io {
    class BodyReader {

    public:
        BodyReader();
        virtual ~BodyReader();

        /**
         * @brief Reads bodies from given input file and calls the particleGenerator to create the right Particles
         * # lines at the beginning are comments
         * in a single line there are the amount of shapes (lines) coming (referred to as n)
         * After that the lines next n lines have follow one of the following pattern: 
         * [fixpoint (vector)] [velocity (vector)] [mass] 
         * [fixpoint (vector)] [velocity (vector)] [mass] [Shape] [dimensions (vector)]     distance
         * After that comments are allowed again (starting with a #)
         * the next non-comment line contains epsilon and sigma
         * example: 
         * 0.0 0.0 0.0      0.0 0.0 0.0     1.0
         * 0.0 0.0 0.0      0.0 0.0 0.0     1.0   Cuboid   4 5 6    1.0
         * # epsilon    sigma
         * 0.1          0.2
         * @param filename is the path to the input file
         * @param buffer is where all generated particles will be stored in
         * @param eps, location for epsilon
         * @param sig, location for sigma
         */
        static void readFile( const char *filename, std::list<Particle>& buffer, double& eps, double& sig);

        /**
         * @brief Converts the String describing a shape to the according shape
         * not case sensitive
         * helper function
         * @param shape 
         * @return enum Shape 
         */
        static enum Shape shapeFromString(std::string& shape);
    };
} // io