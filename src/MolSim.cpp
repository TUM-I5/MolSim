
#include "./inputReader/FileReader.h"
#include "./model/ParticleContainer.h"
#include "./simulation/GravitySimulation.h"
#include "spdlog/sinks/basic_file_sink.h" 

#include <iostream>
#include <list>
#include <memory>

double end_time;
double delta_t;

ParticleContainer particleContainer;
std::list<std::shared_ptr<spdlog::logger>> loggers; 

/**
 * function to initialize all loggers, must be called before creating any other classes
*/
void initializeLoggers();

/**
 * function to deactivate all loggers
*/
void setLogLevel(spdlog::level::level_enum level); 

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 4) {
    std::cout << "Erroneous program call! " << std::endl;
    std::cout << "Usage: ./MolSim filename t_end delta_t" << std::endl;
    return EXIT_FAILURE;
  }

  end_time = std::__cxx11::stod(argsv[2]);
  delta_t = std::__cxx11::stod(argsv[3]);

  initializeLoggers(); 

  // creating a new InputReader which in this case is the FileReader provided
  std::unique_ptr<InputReader> inputReader = std::make_unique<FileReader>();
  inputReader->readInput(particleContainer, argsv[1]);
  
  // initializing the GravitySimulation which calculates forces according with assignment 1
  std::unique_ptr<Simulation> simulation = std::make_unique<GravitySimulation>(GravitySimulation(&particleContainer, end_time, delta_t)); 
  simulation->simulate(); 

  // Under VisualStudio, this must be called before main finishes to workaround a known VS issue
  spdlog::drop_all(); 

  return EXIT_SUCCESS;
}

void initializeLoggers(){
  try 
    {
      auto simulation_logger = spdlog::basic_logger_mt("simulation_logger", "../logs/simulation.txt", true);
      loggers.emplace_back(simulation_logger); 
      auto input_logger = spdlog::basic_logger_mt("input_logger", "../logs/input.txt", true);
      loggers.emplace_back(input_logger); 
      auto output_logger = spdlog::basic_logger_mt("output_logger", "../logs/output.txt", true);
      loggers.emplace_back(output_logger);
      auto memory_logger = spdlog::basic_logger_mt("memory_logger", "../logs/memory.text", true); 
      loggers.emplace_back(memory_logger); 
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}

void setLogLevel(spdlog::level::level_enum level){
  try{
    for(auto logger : loggers){
      spdlog::set_level(level);
    }
  } 
  catch (const spdlog::spdlog_ex& ex)
  {
      std::cout << "Log deactivation failed: " << ex.what() << std::endl;
  }
}
