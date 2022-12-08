/*
 * InputFacade.h
 *
 *  Created on: 17.11.2022
 *      Author: borisov
 */

#include "./InputFacade.h"
#include "../utils/Input.h"
#include "./FileReader.h"
#include "./CuboidInputReader.h"
#include "./SphereInputReader.h"
#include "./XMLInputReader.h"

#include <fstream>
#include <iostream>

/*
 * Constructor and destructor
 */

InputFacade::InputFacade()
{
    fileReader = std::make_unique<FileReader>();
    cuboidInputReader = std::make_unique<CuboidInputReader>();
    sphereInputReader = std::make_unique<SphereInputReader>();
    xmlInputReader = std::make_unique<XMLInputReader>();
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("InputFacade generated!");
}

InputFacade::~InputFacade()
{
    _memoryLogger->info("InputFacade destructed!");
}

void InputFacade::readInput(ProgramParameters &programParameters, const char *filename)
{
    std::string filenameStr = filename;

    bool isXml = Input::isXML(filename);

    if (isXml)
    {
        xmlInputReader->readInput(programParameters, filename);
    }
    else
    {
        std::ifstream input_file(filename);
        std::string tmp_string;

        if (input_file.is_open())
        {
            getline(input_file, tmp_string);

            if (tmp_string == "$Cub")
            {
                cuboidInputReader->readInput(programParameters, filename);
            }
            else if (tmp_string == "$Sph")
            {
                sphereInputReader->readInput(programParameters, filename);
            }
            else
            {
                fileReader->readInput(programParameters, filename);
            }
        }
    }
}