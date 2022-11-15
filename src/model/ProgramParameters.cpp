
#include "ProgramParameters.h"
#include "spdlog/spdlog.h"

#include <iostream>


ProgramParameters::ProgramParameters(){
      initializeLoggers();  
}

const void ProgramParameters::readFromFile(char* filename){
  _inputReader->readInput(_particleContainer, filename); 
}

const void ProgramParameters::readCuboid(){
  //TODO
}



const void ProgramParameters::initializeLoggers(){
  try 
    {
      auto simulation_logger = spdlog::basic_logger_mt("simulation_logger", "../../logs/simulation.txt", true);
      _loggers.emplace_back(simulation_logger); 
      auto input_logger = spdlog::basic_logger_mt("input_logger", "../../logs/input.txt", true);
      _loggers.emplace_back(input_logger); 
      auto output_logger = spdlog::basic_logger_mt("output_logger", "../../logs/output.txt", true);
      _loggers.emplace_back(output_logger);
      auto memory_logger = spdlog::basic_logger_mt("memory_logger", "../../logs/memory.text", true); 
      _loggers.emplace_back(memory_logger); 
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}

const void ProgramParameters::setLogLevel(spdlog::level::level_enum level){
  try{
    for(auto logger : _loggers){
      spdlog::set_level(level);
    }
  } 
  catch (const spdlog::spdlog_ex& ex)
  {
      std::cout << "Log deactivation failed: " << ex.what() << std::endl;
  }
}

const void ProgramParameters::resetParameters(){
  _particleContainer.resetParticles(); 
  _end_time = 100; 
  _delta_t = 0.014; 
}

const void ProgramParameters::setEndTime(double end_time){ _end_time = end_time; } 
const void ProgramParameters::setDeltaT(double delta_t){ _delta_t = delta_t; } 
const void ProgramParameters::setShowMenu(bool showMenu) { _showMenu = showMenu; }
const bool ProgramParameters::getShowMenu() const { return _showMenu; }
ParticleContainer *ProgramParameters::getParticleContainer() { return &_particleContainer; }
const double ProgramParameters::getEndTime() const { return _end_time; } 
const double ProgramParameters::getDeltaT() const { return _delta_t; }

