
#include "spdlog/spdlog.h"
#include "ConsoleMenu.h"

#include <set>
#include <sstream>
#include <iostream>


ConsoleMenu::ConsoleMenu(ProgramParameters *programParameters){
    _programParameters = programParameters;
}

const void ConsoleMenu::openMenu(){
  printHelpMenu();
  while(1){
    std::string command; 
    std::cout << "MolSim Group G > Enter command: "; 
    std::getline(std::cin, command); 
    if(!verifyCommand(command)){
      printHelpMenu(); 
    } else {
      char c = command[1]; 

      switch(c) {
        case 'f': 
          std::string parameter = command.substr(3); 
      }
    }
  }
}

const bool ConsoleMenu::verifyCommand(std::string command) const {
  if(command.length() < 2 || command[0] != '-'){
    return false; 
  }
  std::set<char> commandsWithParameters = {'f', 't', 'd', 'l'}; 
  if(commandsWithParameters.count(command[1]) != 0){
    return (command.length() > 3); 
  }

  return true; 
}

const void ConsoleMenu::printHelpMenu() const{
  printf("MolSim Group G > Welcome to the menu of the MolSim application. In here, you can set parameters of the application, read in multiple inputs, and finally run it\n"); 
  printf("MolSim Group G > -f <filename> .......... The path to an input file. If not specified, ../input/default.txt is used to run the program\n");
  printf("MolSim Group G > -c ..................... Add a cuboid to the simulation\n");
  printf("MolSim Group G > -t <end_time> .......... The end time of the simulation. If not specified, 100 is used\n");
  printf("MolSim Group G > -d <delta_t> ........... The size of the time steps in the simulation. If not specified 0.014 is used\n");
  printf("MolSim Group G > -l <log_level>.......... Activate '1' or deactivate '0' the loggers. If not specified, the loggers are activated\n");
  printf("MolSim Group G > -m ..................... Enter the console menu, here you can read in multiple files, create multiple cuboids and re-run the program with the same parameters\n");
  printf("MolSim Group G > -x ..................... Reset the program to the particles from ../input/default.txt, end_time = 100 and delta_t = 0.014\n");  
  printf("MolSim Group G > -r ..................... Run the program with the currently set values\n");  
  printf("MolSim Group G > -h ..................... Help for the menu\n");
  printf("MolSim Group G > -m ..................... Quit the program\n");
}