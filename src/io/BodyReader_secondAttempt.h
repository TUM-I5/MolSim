#pragma once

#include "Body.h"
#include "ParticleGenerator.h"

#include <list>
#include <Eigen>

namespace io {
    class BodyReader {

    public:
        BodyReader();
        virtual ~BodyReader();

        /**
         * @brief Reads bodies from given input file and calls the particleGenerator to create the right Particles
         * # lines at the beginning are comments
         * in a single line there are the amount of shapes (lines) coming
         * After that the lines have follow one of the following pattern: 
         * [fixpoint (vector)] [velocity (vector)] [mass] 
         * [fixpoint (vector)] [velocity (vector)] [mass] [Shape] [dimensions (vector)]
         * example: 
         * 0.0 0.0 0.0      0.0 0.0 0.0     1.0
         * 0.0 0.0 0.0      0.0 0.0 0.0     1.0 Cuboid  4 5 6 
         * @param filename 
         * @param bodies 
         * @return * void 
         */
        void readFile( const char *filename, ParticleGenerator& particleGenerator);

        /**
         * @brief Converts the String describing a shape to the according shape
         * not case sensitive
         * helper function
         * @param shape 
         * @return enum Shape 
         */
        enum Shape shapeFromString(std::string& shape);
    };
} // io