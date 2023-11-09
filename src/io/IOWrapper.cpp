#include "IOWrapper.h"

#include <filesystem>
#include <iostream>

void IOWrapper::readFile(const std::string& input_file_path, ParticleContainer& particle_container) {
    if (input_file_path.empty()) {
        std::cout << "Error: no input file path given." << std::endl;
        exit(-1);
    }

    std::string file_extension = input_file_path.substr(input_file_path.find_last_of(".") + 1);
    if (file_extension == "ps") {
        txt_file_reader.readFile(input_file_path, particle_container);
    } else if (file_extension == "cub") {
        cub_file_reader.readFile(input_file_path, particle_container);
    } else {
        std::cout << "Error: unknown file extension '" << file_extension << "'." << std::endl;
        exit(-1);
    }
}

void IOWrapper::writeFile(const std::string& output_dir_path, const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container) {
    if (!std::filesystem::exists(output_dir_path)) {
        std::filesystem::create_directories(output_dir_path);
    }

    vtk_writer.initializeOutput(particle_container.size());

    for (const Particle& particle : particle_container) {
        vtk_writer.plotParticle(particle);
    }

    vtk_writer.writeFile(output_dir_path + "/" + output_file_name_base, iteration);
}