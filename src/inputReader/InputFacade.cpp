/*
 * InputFacade.h
 *
 *  Created on: 17.11.2022
 *      Author: borisov
 */

#include "./InputFacade.h"

#include <fstream>

/*
* Constructor and destructor
*/

InputFacade::InputFacade() {
    this->fileReader = new FileReader();
    this->cuboidInputReader = new CuboidInputReader();
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("InputFacade generated!");
}

InputFacade::~InputFacade() {
    _memoryLogger->info("InputFacade destructed!");
}

void InputFacade::readInput(ParticleContainer& particleContainer, const char *filename) {
    std::ifstream input_file(filename);
    std::string tmp_string;

    if (input_file.is_open()) {
        getline(input_file, tmp_string);

        if (tmp_string == "$Cub") {
            cuboidInputReader->readInput(particleContainer, filename);
        } else {
            fileReader->readInput(particleContainer, filename);
        }
    }
}

/*
* Getters
*/

const InputReader* InputFacade::getFileReader() {
    return this->fileReader;
}

const InputReader* InputFacade::getCuboidInputReader() {
    return this->cuboidInputReader;
}