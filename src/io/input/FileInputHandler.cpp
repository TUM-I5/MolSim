#include "FileInputHandler.h"

#include <filesystem>
#include <iostream>

#include "io/logger/Logger.h"

void FileInputHandler::readFile(const std::string& input_file_path, DirectSumContainer& particle_container) const {
    if (input_file_path.empty()) {
        Logger::logger->error("No input file specified.");
        exit(-1);
    }

    const std::string file_extension = input_file_path.substr(input_file_path.find_last_of("."));

    try {
        if (file_extension == ".ps") {
            ps_file_reader.readFile(input_file_path, particle_container);
        } else if (file_extension == ".cub") {
            cub_file_reader.readFile(input_file_path, particle_container);
        } else {
            Logger::logger->error("Error: unknown file extension '{}'.", file_extension);
            exit(-1);
        }
    } catch (const FileReader::FileFormatException& e) {
        Logger::logger->critical("Program terminated after throwing an instance of 'FileReader::FileFormatException'.");
        exit(-1);
    }
}
