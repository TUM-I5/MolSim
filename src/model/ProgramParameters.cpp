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
    _sigma = 1;
    _epsilon = 5;
    _benchmark_iterations = 0;
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
const void ProgramParameters::setBenchmarkIterations(int iterations) { _benchmark_iterations = iterations; }
const void ProgramParameters::setSigma(double sigma) { _sigma = sigma; }
const void ProgramParameters::setEpsilon(double epsilon) { _epsilon = epsilon; }
const void ProgramParameters::setShowMenu(bool show_menu) { _showMenu = show_menu; }
const int ProgramParameters::getBenchmarkIterations() const { return _benchmark_iterations; }
ParticleContainer *ProgramParameters::getParticleContainer() { return &_particleContainer; }
const double ProgramParameters::getEndTime() const { return _end_time; }
const double ProgramParameters::getDeltaT() const { return _delta_t; }
const double ProgramParameters::getSigma() const { return _sigma; }
const double ProgramParameters::getEpsilon() const { return _epsilon; }
const bool ProgramParameters::getShowMenu() const { return _showMenu; }
