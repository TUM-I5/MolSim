
#pragma once 

#include "spdlog/spdlog.h"
#include "./model/ParticleContainer.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include "./model/ProgramParameters.h"

#include <list>


class ConsoleMenu{
private:
    ProgramParameters *_programParameters; 
     
    const void printHelpMenu() const; 

    const bool verifyCommand(std::string command) const; 

public: 
    ConsoleMenu(ProgramParameters *_programParameters); 

    const void openMenu();
};