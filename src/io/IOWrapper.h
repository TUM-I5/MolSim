#pragma once

#include "io/input/FileReader.h"
#include "io/output/VTKWriter.h"

/**
 * @brief IO wrapper class to abstract reading and writing of files
 *
 * This class abstracts the reading and writing of files, so that the Simulation class does not have to know about the concrete implementations
 * Currently uses hard coded FileReader and VTKWriter
 */
class IOWrapper {
    FileReader file_reader;
    VTKWriter vtk_writer;

   public:
    void readFile(std::string& filename, ParticleContainer& particle_container);
    void writeFile(std::string& filename, int iteration, ParticleContainer& particle_container);
};