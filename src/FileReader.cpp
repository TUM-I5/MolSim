/*
 * FileReader.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#include "FileReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

FileReader::FileReader() {}

FileReader::~FileReader() {}

void FileReader::readFile(std::list<Particle> &particles, char *filename) {
  std::array<double, 3> x{0., 0., 0.};
  std::array<double, 3> v{1., 1., 1.};
  double m = 1.;
  int num_particles = 0;

  std::ifstream input_file(filename);
  string tmp_string;

  if (input_file.is_open()) {

    getline(input_file, tmp_string);
    cout << "Read line: " << tmp_string << endl;

    while (tmp_string.size() == 0 || tmp_string[0] == '#') {
      getline(input_file, tmp_string);
      cout << "Read line: " << tmp_string << endl;
    }

    istringstream numstream(tmp_string);
    numstream >> num_particles;
    cout << "Reading " << num_particles << "." << endl;
    getline(input_file, tmp_string);
    cout << "Read line: " << tmp_string << endl;

    for (int i = 0; i < num_particles; i++) {
      istringstream datastream(tmp_string);

      for (int j = 0; j < 3; j++) {
        datastream >> x[j];
      }
      for (int j = 0; j < 3; j++) {
        datastream >> v[j];
      }
      if (datastream.eof()) {
        cout
            << "Error reading file: eof reached unexpectedly reading from line "
            << i << endl;
        exit(-1);
      }
      datastream >> m;
      particles.emplace_back(x, v, m);

      getline(input_file, tmp_string);
      cout << "Read line: " << tmp_string << endl;
    }
  } else {
    std::cout << "Error: could not open file " << filename << std::endl;
    exit(-1);
  }
}
