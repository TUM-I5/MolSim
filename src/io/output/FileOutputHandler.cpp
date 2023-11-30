#include "FileOutputHandler.h"

#include <filesystem>
#include <iostream>

#include "io/logger/Logger.h"

FileOutputHandler::FileOutputHandler(const OutputFormat output_format, const std::string& output_dir_path)
    : output_format(output_format), output_dir_path(output_dir_path) {
    switch (output_format) {
        case OutputFormat::VTK:
            file_writer = std::make_unique<VTKWriter>();
            break;
        case OutputFormat::XYZ:
            file_writer = std::make_unique<XYZWriter>();
            break;
        case OutputFormat::NONE:
            return;
        default:
            Logger::logger->error("Output format not implemented.");
            exit(1);
    }

    if (std::filesystem::exists(output_dir_path)) {
        std::filesystem::remove_all(output_dir_path);
    }
    std::filesystem::create_directories(output_dir_path);
}

void FileOutputHandler::writeFile(int iteration, const std::unique_ptr<ParticleContainer>& particle_container) const {
    if (output_format == OutputFormat::NONE) {
        return;
    }
    file_writer->writeFile(output_dir_path, iteration, particle_container);
}
