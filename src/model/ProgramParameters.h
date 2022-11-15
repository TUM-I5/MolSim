
#pragma once 


#include "../model/ParticleContainer.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include "../inputReader/InputReader.h"
#include "../inputReader/FileReader.h"
#include "spdlog/spdlog.h"

#include <list>

class ProgramParameters{
private: 
    ParticleContainer _particleContainer; 
    double _end_time = 100; 
    double _delta_t = 0.014; 
    std::list<std::shared_ptr<spdlog::logger>> _loggers; 
    bool _showMenu= false;
    InputReader *_inputReader = new FileReader();

    /**
     * function to initialize all loggers, must be called before creating any other classes
    */
    const void initializeLoggers();

public: 
    ProgramParameters(); 

    const void readFromFile(char* filename); 

    const void readCuboid(); 

    const void runWithCurrentParameters(); 
    
    /**
    * function to set log level for every logger
    */
    const void setLogLevel(spdlog::level::level_enum level); 


    const void resetParameters(); 

    const void setEndTime(double end_time); 
    const void setDeltaT(double delta_t); 
    const void setShowMenu(bool showMenu); 
    const bool getShowMenu() const; 
    ParticleContainer *getParticleContainer(); 
    const double getEndTime() const; 
    const double getDeltaT() const; 

};