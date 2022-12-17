/*
 *  InterParticleForce.cpp
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#include "InterParticleForce.h"

InterParticleForce::InterParticleForce()
{
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("ForceCalculation generated!");
}

InterParticleForce::~InterParticleForce()
{
    _memoryLogger->info("ForcCalculation destructed!");
}