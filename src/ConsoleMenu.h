/*
 *  ConsoleMenu.h
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#pragma once

#include "spdlog/spdlog.h"
#include "./model/ParticleContainer.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "./model/ProgramParameters.h"
#include "./inputReader/InputFacade.h"
#include "./utils/Input.h"
#include "./utils/Logger.h"

#include <list>

/**
 * @brief A class encapsulating the console menu which allows interaction with the program
 */
class ConsoleMenu
{
private:
    ProgramParameters *_programParameters;         /// used to set and get the parameters
    std::shared_ptr<spdlog::logger> _memoryLogger; /// a speedlog logger which logs construction and destruction of particles
    InputFacade *_inputFacade;                      /// a facade that handles all the input files

    /**
     * @brief prints the help for the console menu
     */
    const void printHelpMenu() const;

    /**
     * @brief verifies if a command is valid
     * @param command which is checked for validity
     * @returns true if the command is valid, false otherwise
     */
    const bool verifyCommand(std::string command) const;

public:
    /**
     * @brief Constructor for the ConsoleMenu
     * @param programParameter A wrapper for all the parameters of the program
     * @param inputFacade Handles all the input
    */
    ConsoleMenu(ProgramParameters *programParameters, InputFacade *inputFacade);
    ~ConsoleMenu();

    /**
     * @brief opens the console menu
     */
    const void openMenu();
};