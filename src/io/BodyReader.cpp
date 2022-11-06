#include "BodyReader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <strings.h>
#include <algorithm>
#include <vector>
#include <boost/algorithm/string.hpp>

//Adjust the delimiter freely if convention so desires 
//The delimiter is used to split the arguments of the body
const char delimiter = ';';

//Adjust the delimiter_vector freely if convention so desires 
//The delimiter_vector is used to split the arguments of a vector
const char delimiter_vector = ',';

namespace io{
    void BodyReader::readFile(const char *filename, std::list<struct Body> &bodies){
        std::string next_line;

        std::ifstream input_file(filename);
        if(!input_file){std::cerr << "Couldn't open Body input files, Continuing with the programm"; return; }   //input_file.isopen()?

        while(getline(input_file, next_line)){
            

            next_line.erase(remove(next_line.begin(), next_line.end(), ' '), next_line.end());  //delete whitespaces

            struct Body cur_body;
            std::vector<std::string> arguments;
            boost::split(arguments, next_line, [](char c){return c == delimiter;});
            if(arguments.size() != 6) {std::cerr<<"Invalid amount of arguments in line \n %s \n of %s", next_line, filename; continue();}

            cur_body.shape = shapeFromString(arguments[0]);
            cur_body.fixpoint = vectorFromString(arguments[1]);
            cur_body.dimensions = vectorFromString(arguments[2]);
            cur_body.distance = std::stod(arguments[3]);
            cur_body.mass = std::stod(arguments[4]);
            cur_body.start_velocity = vectorFromString(arguments[5]);
            bodies.emplace_back(cur_body);
        }
        input_file.close();
    }

    enum Shape shapeFromString(std::string& shape){
        const size_t number_of_shapes = 2;
        const std::array<std::string, number_of_shapes> strOfShape = {"Cuboid", "Sphere"};
        for(size_t i = 0; i < number_of_shapes; i++){
            if(boost::iequals(shape, strOfShape[i])){
                return all_shapes[i];   //all shapes defined in Body.h
            } 
        }
        std::cerr<< "Couldn't interpret %s as shape",shape;
        exit(-1);
    }

    Eigen::Vector3d vectorFromString(std::string& vector){
        std::vector<std::string> arguments;
        Eigen::Vector3d returnvalue;
        boost::split(arguments, vector, [](char c){return c == delimiter_vector;});
        if(arguments.size() != 3){std::cerr << "%s is not a valid 3d vector", vector; exit(-1);}
        returnvalue[0] = std::stod((arguments[0][0] == '(') ? arguments[0].erase(0,1) : arguments[0]);
        returnvalue[1] = std::stod(arguments[1]);
        returnvalue[2] = std::stod((arguments[2][-1] == ')') ? arguments[2].erase(vector.size()-1) : arguments[0]);
    }
}
