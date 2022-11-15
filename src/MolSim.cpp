
#include "./inputReader/FileReader.h"
#include "./model/ParticleContainer.h"
#include "./model/ProgramParameters.h"
#include "./simulation/Simulation.h"
#include "ConsoleMenu.h"
#include "spdlog/sinks/basic_file_sink.h" 

#include <iostream>
#include <list>
#include <set>
#include <memory>
#include <unistd.h>
#include <sstream>
#include <algorithm>


ProgramParameters programParameters = ProgramParameters(); 

void printHelp(); 

const void handleInput(int argc, char *argsv[]);

int main(int argc, char *argsv[]) {

  handleInput(argc, argsv);

  if(programParameters.getShowMenu()){
    ConsoleMenu consoleMenu = ConsoleMenu(&programParameters);
    consoleMenu.openMenu(); 
  } else {
    Simulation simulation = Simulation(&programParameters);
    simulation.simulate(); 
  }
  return EXIT_SUCCESS;
}

const void handleInput(int argc, char *argsv[]){
  while (1)
  {
    int result = getopt(argc, argsv, "mhl:t:f:d:");

    if(result == -1){
      break;
    }
    switch (result)
    {
      case '?': 
        printHelp(); 
        exit(1); 
      case ':': 
        printHelp(); 
        exit(1); 
      case 'h':
        printHelp(); 
        exit(0); 
      case 'm': 
        programParameters.setShowMenu(true); 
        break; 
      case 't': 
        programParameters.setEndTime(std::__cxx11::stod(optarg));
        break; 
      case 'd': 
        programParameters.setEndTime(std::__cxx11::stod(optarg)); 
       break; 
      case 'f': 
        programParameters.readFromFile(optarg);
        break; 
      case 'l': {
        int level = std::__cxx11::stoi(optarg);
        if(level == 0){
          programParameters.setLogLevel(spdlog::level::off); 
        } else if(level != 1){
          printHelp(); 
          exit(1); 
        }
        break; 
        }
      default:
        break;
    }
  }
}

void printHelp(){
  printf(" -f <filename> .......... The path to an input file. If not specified and no cuboids are generated, no particles appear in the simulation.\n");
  printf(" -t <end_time> .......... The end time of the simulation. If not specified, 100 is used\n");
  printf(" -d <delta_t> ........... The size of the time steps in the simulation. If not specified 0.014 is used\n");
  printf(" -l <log_level>.......... Activate '1' or deactivate '0' the loggers. If not specified, the loggers are activated\n");
  printf(" -m ..................... Enter the console menu, here you can read in multiple files, create multiple cuboids and re-run the program with the same parameters\n");
  printf(" -h ..................... Help\n");
}
