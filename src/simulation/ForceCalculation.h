
#pragma once

#include "../model/ParticleContainer.h"

class ForceCalculation{
private: 
    /**
     * a speedlog logger which logs construction and destruction of particles 
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;
public: 
    ForceCalculation(); 
    ~ForceCalculation(); 
    
    virtual void calculateForce(ParticleContainer &particleContainer) = 0; 
};