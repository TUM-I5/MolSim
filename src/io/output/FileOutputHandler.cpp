#include "FileOutputHandler.h"

#include <filesystem>
#include <iostream>

void FileOutputHandler::writeFile(const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container) {
    switch (output_format) {
        case OutputFormat::VTK:
            writeVTKFile(output_dir_path, output_file_name_base, iteration, particle_container);
            break;
        case OutputFormat::XYZ:
            writeXYZFile(output_dir_path, output_file_name_base, iteration, particle_container);
            break;
        case OutputFormat::NONE:
            break;
        default:
            std::cerr << "Output format not implemented." << std::endl;
            exit(1);
    }
}

void FileOutputHandler::writeVTKFile(const std::string& output_dir_path, const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container) {
    VTKWriter vtk_writer;

    if (!std::filesystem::exists(output_dir_path)) {
        std::filesystem::create_directories(output_dir_path);
    }

    vtk_writer.initializeOutput(particle_container.size());

    for (const Particle& particle : particle_container) {
        vtk_writer.plotParticle(particle);
    }

    vtk_writer.writeFile(output_dir_path + "/" + output_file_name_base, iteration);
}

void FileOutputHandler::writeXYZFile(const std::string& output_dir_path, const std::string& output_file_name_base, int iteration, const ParticleContainer& particle_container) {
    XYZWriter xyz_writer;

    if (!std::filesystem::exists(output_dir_path)) {
        std::filesystem::create_directories(output_dir_path);
    }

    xyz_writer.plotParticles(particle_container, output_dir_path + "/" + output_file_name_base, iteration);
}
