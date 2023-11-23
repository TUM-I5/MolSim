#include "FileInputHandler.h"

#include <filesystem>
#include <iostream>

#include "io/logger/Logger.h"

std::optional<SimulationParams> FileInputHandler::readFile(const std::string& input_file_path,
                                                           ParticleContainer& particle_container) const {
    if (input_file_path.empty()) {
        Logger::logger->error("No input file specified.");
        exit(-1);
    }

    const std::string file_extension = input_file_path.substr(input_file_path.find_last_of("."));

    try {
        if (file_extension == ".ps") {
            ps_file_reader.readFile(input_file_path, particle_container);
            return std::nullopt;
        } else if (file_extension == ".cub") {
            cub_file_reader.readFile(input_file_path, particle_container);
            return std::nullopt;
        } else if (file_extension == ".xml") {
            SimulationParams params = xml_file_reader.readConfiguration(input_file_path, particle_container);
            return std::make_optional(params);
        } else {
            Logger::logger->error("Error: unknown file extension '{}'.", file_extension);
            exit(-1);
        }
    } catch (const FileFormatException& e) {
        Logger::logger->critical("Program terminated after throwing an instance of 'CustomFileReader::FileFormatException'.");
        exit(-1);
    }
}
