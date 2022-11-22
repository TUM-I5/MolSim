/*
 * ProgramParameters.cpp
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#include "ProgramParameters.h"
#include "spdlog/spdlog.h"

#include <iostream>

ProgramParameters::ProgramParameters()
{
    _particleContainer = ParticleContainer();
    _inputFacade = std::make_unique<InputFacade>();
    _end_time = 100;
    _delta_t = 0.014;
    _showMenu = false;
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("ProgramParameters generated!");
}

ProgramParameters::~ProgramParameters()
{
    _memoryLogger->info("ProgramParameters destructed!");
}

const void ProgramParameters::readFromFile(const char *filename)
{
    _inputFacade->readInput(_particleContainer, filename);
}

const void ProgramParameters::resetParameters()
{
    _particleContainer.resetParticles();
}

const void ProgramParameters::setEndTime(double end_time) { _end_time = end_time; }
const void ProgramParameters::setDeltaT(double delta_t) { _delta_t = delta_t; }
const void ProgramParameters::setShowMenu(bool showMenu) { _showMenu = showMenu; }
const bool ProgramParameters::getShowMenu() const { return _showMenu; }
ParticleContainer *ProgramParameters::getParticleContainer() { return &_particleContainer; }
const double ProgramParameters::getEndTime() const { return _end_time; }
const double ProgramParameters::getDeltaT() const { return _delta_t; }
