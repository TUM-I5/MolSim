#pragma once

#include "Body.h"

#include <list>
#include <Eigen>

namespace io {
    class BodyReader {

    public:
        BodyReader();
        virtual ~BodyReader();

        /**
         * @brief Reads bodies from given input file and writes them into the given list
         * Every file has to consist of lines that follow the following pattern: 
         * [Shape] ([fixpoint]) ([dimensions]) [distance] [mass] ([start_velocity])
         * example: Cuboid (0.1,0.2,0.3) (40,8,1) 1.1225 1.1 ((0.4,0.5,0.6))
         * @param filename 
         * @param bodies 
         * @return * void 
         */
        static void readFile( const char *filename, std::list<struct Body> &bodies);

        
    };
        /**
         * @brief Converts the String describing a shape to the according shape
         * not case sensitive
         * helper function
         * @param shape 
         * @return enum Shape 
         */
        enum Shape shapeFromString(std::string& shape);

        Eigen::Vector3d vectorFromString(std::string& vector);
} // io