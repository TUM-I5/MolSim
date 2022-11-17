
#include "spdlog/spdlog.h"
#include "ConsoleMenu.h"
#include "./simulation/Simulation.h"
#include "./utils/Input.h"

#include <set>
#include <sstream>
#include <iostream>
#include <regex>
#include <fstream> 


ConsoleMenu::ConsoleMenu(ProgramParameters *programParameters){
  _programParameters = programParameters;
  _memoryLogger = spdlog::get("memory_logger");
  _memoryLogger->info("ConsoleMenu generated!");
}



const void ConsoleMenu::openMenu(){
  printHelpMenu();
  while(1){
    std::string command; 
    std::cout << "MolSim Group G > Enter command: "; 
    std::getline(std::cin, command); 
    command = Input::ftrim(command); 
    if(!verifyCommand(command)){
      printHelpMenu(); 
    } else {
      char c = command[1]; 
      std::string parameter;

      switch(c) {
        case 'f': 
          parameter = Input::trim(Input::trim(command.substr(2)), "\""); 
          _programParameters->readFromFile(parameter.c_str());
          break; 
        case 't': 
          parameter = Input::trim(command.substr(2)); 
          _programParameters->setEndTime(std::__cxx11::stod(parameter));
          break;
        case 'd': 
          parameter = Input::trim(command.substr(2)); 
          _programParameters->setDeltaT(std::__cxx11::stod(parameter));
          break;
        case 'l' : {
          parameter = Input::trim(command.substr(2));
          int level = std::__cxx11::stoi(parameter); 
          if(level == 0){
            _programParameters->setLogLevel(spdlog::level::off);
          } else {
            _programParameters->setLogLevel(spdlog::level::info);
          }
          }
          break; 
        case 'x':  
          _programParameters->resetParameters(); 
          break; 
        case 'r': {
          std::cout << "MolSim Group G > Running simulation ... ";
          Simulation simulation = Simulation(_programParameters);
          simulation.simulate(); 
          std::cout << "Finished" << std::endl; 
          }
          break; 
        case 'h': 
          printHelpMenu(); 
          break; 
        case 'c': 
          break; 
        case 'q': 
          std::cout << "MolSim Group G > Quitting ..." << std::endl;
          return;  
      }
    }
  }
}

ConsoleMenu::~ConsoleMenu() {
  _memoryLogger->info("ConsoleMenu destructed!");
  _memoryLogger->flush(); 
}

const bool ConsoleMenu::verifyCommand(std::string command) const {
  std::set<char> commands = {'f', 'c', 't', 'd', 'l', 'x', 'r', 'h', 'q'}; 
  if(command.length() < 2 || command[0] != '-' || commands.count(command[1]) == 0){
    return false; 
  }
  
  std::set<char> commandsWithParameters = {'f', 't', 'd', 'l'}; 
  bool parameterTest; 
  if(commandsWithParameters.count(command[1]) != 0){
    switch(command[1]){
    case 'l': {
      parameterTest = Input::isInt(Input::trim(command.substr(2)));
      if(!parameterTest){
        std::cout << "MolSim Group G > Error: Parameter is not an int" << std::endl;
        return false;  
      } 
      parameterTest = Input::isValidLevel(Input::trim(command.substr(2))); 
      if(!parameterTest){
        std::cout << "MolSim Group G > Error: Only 0 and 1 are valid log levels" << std::endl;
        return false;  
      }
      }
      return true; 
    case 'f': {
      std::ifstream test(Input::trim(Input::trim(command.substr(2)), "\""));
      if(!test){
        std::cout << "MolSim Group G > Error: File does not exist" << std::endl; 
        return false;
      }
      }
      return true; 
    case 't': 
      parameterTest = Input::isDouble(Input::trim(command.substr(2)));
      if(!parameterTest){
        std::cout << "MolSim Group G > Error: Parameter is not a double" << std::endl;
        return false;  
      }
      return true; 
    case 'd': 
      parameterTest = Input::isDouble(Input::trim(command.substr(2)));
      if(!parameterTest){
        std::cout << "MolSim Group G > Error: Parameter is not a double" << std::endl;
        return false;  
      }
      return true; 
    }
  }
    return command.length() == 2; 
}

const void ConsoleMenu::printHelpMenu() const{
  printf("===============================================================================================================================================================================\n");
  printf("MolSim Group G > Welcome to the menu of the MolSim application. In here, you can set parameters of the application, read in multiple inputs, and finally run it\n"); 
  printf("MolSim Group G > -f <filename> .......... The path to an input file. If not specified, ../input/default.txt is used to run the program\n");
  printf("MolSim Group G > -c ..................... Add a cuboid to the simulation\n");
  printf("MolSim Group G > -t <end_time> .......... The end time of the simulation. If not specified, 100 is used\n");
  printf("MolSim Group G > -d <delta_t> ........... The size of the time steps in the simulation. If not specified 0.014 is used\n");
  printf("MolSim Group G > -l <log_level>.......... Activate '1' or deactivate '0' the loggers. If not specified, the loggers are activated\n");
  printf("MolSim Group G > -x ..................... Deletes all particles from the simulation\n");  
  printf("MolSim Group G > -r ..................... Run the program with the currently set values\n");  
  printf("MolSim Group G > -h ..................... Help for the menu\n");
  printf("MolSim Group G > -q ..................... Quit the program\n");
  printf("===============================================================================================================================================================================\n");
}