#include "IOWrapper.h"

#include <filesystem>

void IOWrapper::readFile(const std::string& input_file_path, ParticleContainer& particle_container) {
    file_reader.readFile(input_file_path, particle_container);
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