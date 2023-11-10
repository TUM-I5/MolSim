#include "FileInputHandler.h"

#include <filesystem>
#include <iostream>

void FileInputHandler::readFile(const std::string& input_file_path, ParticleContainer& particle_container) {
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
