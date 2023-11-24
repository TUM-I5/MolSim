#include "FileInputHandler.h"

#include <filesystem>
#include <iostream>

#include "io/logger/Logger.h"

SimulationParams FileInputHandler::readFile(const std::string& input_file_path,
                                            std::unique_ptr<ParticleContainer>& particle_container) const {
    if (!std::filesystem::exists(input_file_path)) {
        Logger::logger->error("Error: file '{}' does not exist.", input_file_path);
        exit(-1);
    }

    std::string file_extension;

    try {
        file_extension = input_file_path.substr(input_file_path.find_last_of("."));
    } catch (const std::out_of_range& e) {
        Logger::logger->error("Error: no file extension found.");
        exit(-1);
    }

    std::unique_ptr<FileReader> file_reader;

    if (file_extension == ".ps") {
        file_reader = std::make_unique<PsFileReader>();
    } else if (file_extension == ".cub") {
        file_reader = std::make_unique<CubFileReader>();
    } else if (file_extension == ".xml") {
        file_reader = std::make_unique<XMLFileReader>();
    } else {
        Logger::logger->error("Error: unknown file extension '{}'.", file_extension);
        exit(-1);
    }

    try {
        return file_reader->readFile(input_file_path, particle_container);
    } catch (const FileFormatException& e) {
        Logger::logger->error("Error: file format exception.");
        exit(-1);
    }
}
