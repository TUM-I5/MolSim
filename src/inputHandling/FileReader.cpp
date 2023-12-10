
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "FileReader.h"
#include "xmlParsing/parameters.hpp"

#include <spdlog/spdlog.h>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <type_traits>

FileReader::FileReader() = default;

FileReader::~FileReader() = default;





/**
 * @brief Sets a boundary condition based on the specified string.
 *
 * This function sets the boundary condition according to the provided string value.
 * If the specified string matches "reflective" or "outflow", it assigns the corresponding enum value
 * to the 'boundary' parameter.
 *
 * @param boundary Reference to a FileReader::boundary_conditions enum variable where the boundary condition will be set.
 * @param specified_cond A string specifying the desired boundary condition ("reflective" or "outflow"), which normally comes from the xml file.
 *
 * @throws std::invalid_argument If the string is not recognized(not "reflective" or "outflow"), an invalid_argument exception is thrown,
 *                               providing an error message indicating the incorrect string that was given.)
 */
void set_boundary_conditional(boundary_conditions& boundary,std::string specified_cond){
    if(specified_cond == "reflective")
        boundary = boundary_conditions::reflective;
    else if(specified_cond == "outflow")
        boundary = boundary_conditions::outflow;
    else if(specified_cond == "periodic")
        boundary = boundary_conditions::periodic;
    else
        throw std::invalid_argument("The Boundary Conditions were not correctly specified, you gave: " + specified_cond);

}

/**
 * @brief reads xml file and constructs ProgramsArgs struct corresponding to xml file
 * 
 * 
 * Reads an XML file 'filename' and uses Codesynthesis to parse/ validate the xml file then.
 * The information of the object returned from the XML file parser is then writen into an ProgramArgs struct
 * 
 * 
 * @param filename XML file according to parameters.xsd (can be found in input/ folder)
 * 
 * @returns a ProgramArgs struct with the information from the file
 * 
*/
FileReader::ProgramArgs FileReader::readProgramArguments(std::string filename){


    std::unique_ptr<parameters> params = parameters_(filename);


    auto out_params = params->outputParameters();
    auto sim_params = params->simulationParameters();
    auto cuboids = params->cuboids();
    auto spheres = params->spheres();
    auto boundary_conditions_xml = sim_params.boundaryConditions();
    boundary_conditions positive_z, negative_z ,positive_x, negative_x , positive_y , negative_y;

    set_boundary_conditional(positive_z,boundary_conditions_xml.boundaryConditionsPositiveZ());
    set_boundary_conditional(negative_z,boundary_conditions_xml.boundaryConditionsNegativeZ());
    set_boundary_conditional(positive_x,boundary_conditions_xml.boundaryConditionsPositiveX());
    set_boundary_conditional(negative_x,boundary_conditions_xml.boundaryConditionsNegativeX());
    set_boundary_conditional(positive_y,boundary_conditions_xml.boundaryConditionsPositiveY());
    set_boundary_conditional(negative_y,boundary_conditions_xml.boundaryConditionsNegativeY());



    ProgramArgs args;
    args.delta_t = sim_params.deltaT();
    args.t_end = sim_params.tEnd();
    args.cut_of_radius = sim_params.cutOfRadius();
    args.cell_size = sim_params.cellSize();
    args.boundaries = {positive_z,negative_z,positive_x,negative_x,positive_y,negative_y};
    
    args.domain_dimensions = {sim_params.domainDimensions().x(),sim_params.domainDimensions().y(),sim_params.domainDimensions().z()};

    args.file_basename = out_params.baseName();
    args.write_frequency = out_params.writeFrequency();

    
    for(size_t i = 0; i < cuboids.size() ; i++){
        CuboidData c;
        auto cuboid = cuboids[i];
        c.x = { cuboid.position().x(), cuboid.position().y(), cuboid.position().z() };
        c.v = { cuboid.velocity().x(), cuboid.velocity().y(), cuboid.velocity().z() };
    

        c.N1 = cuboid.dimensions().x();
        c.N2 = cuboid.dimensions().y();
        c.N3 = cuboid.dimensions().z();

        c.m = cuboid.mass();
        c.h = cuboid.meshWidth();
        c.sigma = cuboid.sigma();
        c.epsilon = cuboid.epsilon();

        args.cuboids.push_back(c);
    }


    for(size_t i = 0; i < spheres.size() ; i++){
        SphereData s;
        auto sphere = spheres[i];
        s.CenterPosition = { sphere.center_position().x(), sphere.center_position().y(), sphere.center_position().z() };
        s.Velocity = { sphere.velocity().x(), sphere.velocity().y(), sphere.velocity().z() };
        s.mass = sphere.mass();
        s.radius = sphere.radius();
        s.meshWidth = sphere.meshWidth();
        s.sigma = sphere.sigma();
        s.epsilon = sphere.epsilon();

        args.spheres.push_back(s);
    }


    return args;    
}












/**
 * @brief reads a string of an array and parses the string into an array
 *
 * parses the string representation of an array and fills
 * the provided array with the values read from the string. The string should
 * have the format "{x, y, z}", where x, y and z are either int's or double's.
 *
 * @tparam T Type of elements in the array (int or double)
 * @param str string representation of the array
 * @param array reference to the array, which should be filled with the values
 * from the string
 *
 * @throws invalid_argument exception If the input string is too short or is not
 * of the {x,y,z} format
 *
 * @note T has to be either double or uint64_t
 */
template <typename T>
typename std::enable_if<std::is_same<T, double>::value ||
                            std::is_same<T, uint64_t>::value,
                        void>::type
readArrayString(std::string str, std::array<T, 3> &array) {
  auto logger = spdlog::get("logger");
  SPDLOG_LOGGER_DEBUG(logger, "Trying to read this array: " + str + "\n");

  char brace_check;
  char comma_check;
  if (str.length() < 6)
    throw std::invalid_argument("the array string " + str +
                                " was too short to be a valid argument");

  std::string tmp;
  std::istringstream strs(str);
  strs >> std::ws >> brace_check;

  if (brace_check != '{')
    throw std::invalid_argument("array" + str + " has to start with {");

  strs >> std::ws >> array[0];

  strs >> std::ws >> comma_check;
  if (comma_check != ',')
    throw std::invalid_argument("array " + str +
                                " has to contain , between braces");

  strs >> std::ws >> array[1];

  strs >> std::ws >> comma_check;
  if (comma_check != ',')
    throw std::invalid_argument("array " + str +
                                " has to contain , between braces");

  strs >> std::ws >> array[2];
  strs >> std::ws >> brace_check;

  if (brace_check != '}')
    throw std::invalid_argument("array " + str + " has to end with }");
}

/**
 * @brief parses the number after 'name' in 'line'
 *
 * mainly used by readCuboidFile() to search for a keyword specified by name
 * within a line and then tries to parse the number after that keyword into a
 * double
 *
 * @param name keyword, which is being searched
 * @param line entire line (should contain the keyword and a number afterwards)
 * @param err_msg error message that is being thrown, if th keyword is not in
 * the line
 *
 * @return parsed double that stood after the keyword
 *
 * @throws std::invalid_argument If the parameter is not found in the line
 */
double parseParam(std::string name, std::string line, std::string err_msg) {
    auto logger = spdlog::get("logger");
    std::size_t str_index;

    if ((str_index = line.find(name)) != std::string::npos) {
        std::string rest = line.substr(str_index + std::string(name).length());
        SPDLOG_LOGGER_DEBUG(logger, name + " " + rest);
        return std::stod(rest);
    } else {
        SPDLOG_LOGGER_DEBUG(logger, str_index);
        throw std::invalid_argument(err_msg);
    }
}

std::list<FileReader::CuboidData> FileReader::readCuboidFile(std::string filename) {
    auto logger = spdlog::get("logger");

    std::list<CuboidData> data;
    std::ifstream input_file(filename);
    if (input_file.is_open()) {
        std::string line;
        SPDLOG_LOGGER_DEBUG(logger, "Opened file for reading\n");

        while (!input_file.eof()) {
            getline(input_file, line);
            SPDLOG_LOGGER_DEBUG(logger, "read line: " + line);

            if (!(line.empty() or line[0] == '#') and
                (line.find("cuboid:") != std::string::npos)) {
                SPDLOG_LOGGER_DEBUG(logger, "Found another cuboid\n");
                // there is a cuboid
                data.emplace_back();
                CuboidData &param = data.back();

                getline(input_file, line);

                std::size_t str_index;

                // Read the position of the Cuboid
                if ((str_index = line.find("position:")) != std::string::npos) {
                    std::string rest =
                            line.substr(str_index + std::string("position:").length());

                    readArrayString(rest, param.x);
                } else {
                    std::cerr << "Error: cuboid position was not specified in file"
                              << std::endl;
                }

                getline(input_file, line);
                SPDLOG_LOGGER_DEBUG(logger, "read line " + line);

                if ((str_index = line.find("velocity:")) != std::string::npos) {
                    std::string rest =
                            line.substr(str_index + std::string("velocity:").length());

                    readArrayString(rest, param.v);
                } else {
                    std::cerr << "Error: cuboid velocity was not specified in file"
                              << std::endl;
                }

                getline(input_file, line);
                SPDLOG_LOGGER_DEBUG(logger, "read line " + line);

                if ((str_index = line.find("(N1 x N2 x N3):")) != std::string::npos) {
                    std::string rest =
                            line.substr(str_index + std::string("(N1 x N2 x N3):").length());
                    std::array<uint64_t, 3> N{};
                    readArrayString(rest, N);
                    param.N1 = N[0];
                    param.N2 = N[1];
                    param.N3 = N[2];
                } else {
                    std::cerr << "Error: cuboid dimensions(N1 x N2 x N3) were not "
                                 "specified in file"
                              << std::endl;
                }

                getline(input_file, line);
                SPDLOG_LOGGER_DEBUG(logger, "read line: " + line);

                param.m = parseParam(
                        "mass:", line,
                        "Error: mass of Particles within Cuboid was not specified "
                        "in file");

                getline(input_file, line);
                SPDLOG_LOGGER_DEBUG(logger, "read line: " + line);

                param.h =
                        parseParam("mesh-width:", line,
                                   "Error: mesh width Cuboid was not specified in file");

                getline(input_file, line);
                SPDLOG_LOGGER_DEBUG(logger, "read line: " + line);

                param.sigma = parseParam(
                        "sigma:", line, "Error: sigma of Cuboid was not specified in file");

                getline(input_file, line);
                SPDLOG_LOGGER_DEBUG(logger, "read line: " + line);

                param.epsilon =
                        parseParam("epsilon:", line,
                                   "Error: epsilon of Cuboid was not specified in file");
            }
        }
        SPDLOG_LOGGER_DEBUG(logger, "File is closed\n");
        input_file.close();
    } else {
        throw std::runtime_error("Error opening the file.");
    }

    for (auto &cub: data) {
        SPDLOG_LOGGER_DEBUG(logger, "Got these Cuboids: \n{}", cub.to_string());
    }

    logger->flush();

    return data;
}

void FileReader::readParticleFile(ParticleContainer &particleContainer,
                                  char *filename) {
    auto logger = spdlog::get("logger");
    std::array<double, 3> x;
    std::array<double, 3> v;
    double m;
    int num_particles = 0;

    std::ifstream input_file(filename);
    std::string tmp_string;

    if (input_file.is_open()) {
        getline(input_file, tmp_string);
        SPDLOG_LOGGER_DEBUG(logger, "Read line: " + tmp_string +"\n");

        while (tmp_string.empty() or tmp_string[0] == '#') {
            getline(input_file, tmp_string);
            SPDLOG_LOGGER_DEBUG(logger, "Read line: " + tmp_string +"\n");
        }

        std::istringstream numstream(tmp_string);
        numstream >> num_particles;
        std::cout << "Reading " << num_particles << "." << std::endl;
        SPDLOG_LOGGER_DEBUG(logger, "Reading " + std::to_string(num_particles) + ".");
        getline(input_file, tmp_string);
        SPDLOG_LOGGER_DEBUG(logger, "Read line: " + tmp_string +"\n");

        for (int i = 0; i < num_particles; i++) {
            std::istringstream datastream(tmp_string);

            for (auto &xj: x) {
                datastream >> xj;
            }
            for (auto &vj: v) {
                datastream >> vj;
            }
            if (datastream.eof()) {
                SPDLOG_ERROR("Error reading file: eof reached unexpectedly reading from line " + std::to_string(i));
                exit(-1);
            }
            datastream >> m;
            particleContainer.addParticle(x, v, m);

            getline(input_file, tmp_string);
            SPDLOG_LOGGER_DEBUG(logger, "Read line: " + tmp_string + "\n");
        }
    } else {
        SPDLOG_ERROR("Error: could not open file " + std::string(filename));
        exit(-1);
    }
}
