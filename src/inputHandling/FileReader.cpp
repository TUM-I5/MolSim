/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"
#include "CuboidGenerator.h"

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
  if (FileReader::verbose_FileReader)
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
  std::size_t str_index;
  if ((str_index = line.find(name)) != std::string::npos) {
    std::string rest = line.substr(str_index + std::string(name).length());
    if (FileReader::verbose_FileReader)
      std::cout << name + " " + rest << std::endl;
    return std::stod(rest);
  } else {
    if (FileReader::verbose_FileReader) std::cout << str_index << std::endl;
    throw std::invalid_argument(err_msg);
  }
}

std::list<FileReader::CuboidData> FileReader::readCuboidFile(ParticleContainer& particleContainer, char *filename) {
  std::list<CuboidData> data;
  std::ifstream input_file(filename);
  if (input_file.is_open()) {
    std::string line;

    while (!input_file.eof()) {
      getline(input_file, line);
      if (FileReader::verbose_FileReader)
        std::cout << "read line: " << line << std::endl;
      if (!(line.empty() or line[0] == '#') and
          (line.find("cuboid:") != std::string::npos)) {
            std::cout << "Entered Loop body\n";
        // there is a cuboid
        std::cout << "Before List alloc\n";
        data.emplace_back();
        CuboidData &param = data.back();
        std::cout << "After List alloc\n";

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
        if (FileReader::verbose_FileReader)
          std::cout << "read line: " << line << std::endl;

        if ((str_index = line.find("velocity:")) != std::string::npos) {
          std::string rest =
              line.substr(str_index + std::string("velocity:").length());

          readArrayString(rest, param.v);
        } else {
          std::cerr << "Error: cuboid velocity was not specified in file"
                    << std::endl;
        }

        getline(input_file, line);
        if (FileReader::verbose_FileReader)
          std::cout << "read line: " << line << std::endl;

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
        if (FileReader::verbose_FileReader)
          std::cout << "read line: " << line << std::endl;

        param.m = parseParam(
            "mass:", line,
            "Error: mass of Particles within Cuboid was not specified "
            "in file");

        getline(input_file, line);
        if (FileReader::verbose_FileReader)
          std::cout << "read line: " << line << std::endl;

        param.h =
            parseParam("mesh-width:", line,
                       "Error: mesh width Cuboid was not specified in file");

        getline(input_file, line);
        if (FileReader::verbose_FileReader)
          std::cout << "read line: " << line << std::endl;

        param.sigma = parseParam(
            "sigma:", line, "Error: sigma of Cuboid was not specified in file");

        getline(input_file, line);
        if (FileReader::verbose_FileReader)
          std::cout << "read line: " << line << std::endl;

        param.epsilon =
            parseParam("epsilon:", line,
                       "Error: epsilon of Cuboid was not specified in file");
      }
    }
    std::cout << "File is closed";
    input_file.close();
  } else {
    throw std::runtime_error("Error opening the file.");
  }

  //addCuboids(particleContainer, data);
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
