/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"

#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <functional>

FileReader::FileReader() = default;

FileReader::~FileReader() = default;


template <typename T>
void readArrayString(std::string str, std::array<T, 3> &array, bool is_double) {
  std::cout << "Trying to read this array: " << str << "\n";
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

  strs >> std::ws >> tmp;

  if (is_double)
    array[0] = std::stod(tmp);
  else
    array[0] = static_cast<uint64_t>(std::stoi(tmp));

  strs >> std::ws >> comma_check;
  if (comma_check != ',')
    throw std::invalid_argument("array " + str +
                                " has to contain , between braces");

  strs >> std::ws >> tmp;

  if(is_double)
    array[1] = std::stoull(tmp);
  else
    array[1] = static_cast<uint64_t>(std::stoull(tmp));

  strs >> std::ws >> comma_check;
  if (comma_check != ',')
    throw std::invalid_argument("array " + str +
                                " has to contain , between braces");

  strs >> std::ws >> tmp;
  if (is_double)
    array[2] = std::stod(tmp);
  else
    array[2] = static_cast<uint64_t>(std::stoull(tmp));
  strs >> std::ws >> brace_check;

  if (brace_check != '}')
    throw std::invalid_argument("array " + str + " has to end with }");
}



double parseParam(std::string name, std::string line, std::string err_msg) {
  std::size_t str_index;
  if ((str_index = line.find(name)) != std::string::npos) {
    std::string rest = line.substr(str_index + std::string(name).length());
    return std::stod(rest);
  } else {
    std::cout << str_index << std::endl;
    throw std::invalid_argument(err_msg);
  }
}

std::list<FileReader::CuboidData> FileReader::readCuboidFile(char *filename) {
  bool file_verbose = true;
  std::list<CuboidData> data;
  std::ifstream input_file(filename);
  if (input_file.is_open()) {
    std::string line;

    while (!input_file.eof()) {
      getline(input_file, line);
      std::cout << "read line: " << line << std::endl;
      if (!(line.empty() or line[0] == '#') and
          (line.find("cuboid:") != std::string::npos)) {
        // there is a cuboid
        data.emplace_back();
        CuboidData &param = data.back();

        getline(input_file, line);

        std::size_t str_index;

        // Read the position of the Cuboid
        if ((str_index = line.find("position:")) != std::string::npos) {
          std::string rest =
              line.substr(str_index + std::string("position:").length());
          
          readArrayString(rest, param.x,true);
        } else {
          std::cerr << "Error: cuboid position was not specified in file"
                    << std::endl;
        }

        getline(input_file, line);
        if (file_verbose) std::cout << "read line: " << line << std::endl;

        if ((str_index = line.find("velocity:")) != std::string::npos) {
          std::string rest =
              line.substr(str_index + std::string("velocity:").length());
              
          /*lambda expression is needed to tell the compiler which function of std::stod should be used
            so if std::stod is overloaded, the compiler will be able to choose the correct std::stod
          */
          readArrayString(rest, param.v,true);
        } else {
          std::cerr << "Error: cuboid velocity was not specified in file"
                    << std::endl;
        }

        getline(input_file, line);
        if (file_verbose) std::cout << "read line: " << line << std::endl;

        if ((str_index = line.find("(N1 x N2 x N3):")) != std::string::npos) {
          std::string rest =
              line.substr(str_index + std::string("(N1 x N2 x N3):").length());
          std::array<uint64_t, 3> N{};
          readArrayString(rest, N, false);
          param.N1 = N[0];
          param.N2 = N[1];
          param.N3 = N[2];
        } else {
          std::cerr << "Error: cuboid dimensions(N1 x N2 x N3) were not "
                       "specified in file"
                    << std::endl;
        }

        getline(input_file, line);
        if (file_verbose) std::cout << "read line: " << line << std::endl;

        parseParam("mass:", line,
                   "Error: mass of Particles within Cuboid was not specified "
                   "in file");

        getline(input_file, line);
        if (file_verbose) std::cout << "read line: " << line << std::endl;

        parseParam("mesh-width:", line,
                   "Error: mesh width Cuboid was not specified in file");

        getline(input_file, line);
        if (file_verbose) std::cout << "read line: " << line << std::endl;

        parseParam("sigma:", line,
                   "Error: sigma of Cuboid was not specified in file");

        getline(input_file, line);
        if (file_verbose) std::cout << "read line: " << line << std::endl;

        parseParam("epsilon:", line,
                   "Error: epsilon of Cuboid was not specified in file");
      }
    }
    input_file.close();
  } else {
    throw std::runtime_error("Error opening the file.");
  }
  for (auto &cub : data) {
    std::cout << cub.to_string() << std::endl;
  }
  return data;
}

void FileReader::readParticleFile(ParticleContainer &particleContainer,
                                  char *filename) {
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

      for (auto &xj : x) {
        datastream >> xj;
      }
      for (auto &vj : v) {
        datastream >> vj;
      }
      if (datastream.eof()) {
        std::cout
            << "Error reading file: eof reached unexpectedly reading from line "
            << i << std::endl;
        exit(-1);
      }
      datastream >> m;
      particleContainer.addParticle(x, v, m);

      getline(input_file, tmp_string);
      std::cout << "Read line: " << tmp_string << std::endl;
    }
  } else {
    std::cout << "Error: could not open file " << filename << std::endl;
    exit(-1);
  }
}
