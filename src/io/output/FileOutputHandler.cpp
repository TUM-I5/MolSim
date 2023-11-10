#include "FileOutputHandler.h"

#include <filesystem>

void FileOutputHandler::writeFile(const std::string& output_dir_path, const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container) {
    if (!std::filesystem::exists(output_dir_path)) {
        std::filesystem::create_directories(output_dir_path);
    }

    vtk_writer.initializeOutput(particle_container.size());

    for (const Particle& particle : particle_container) {
        vtk_writer.plotParticle(particle);
    }

    vtk_writer.writeFile(output_dir_path + "/" + output_file_name_base, iteration);
}