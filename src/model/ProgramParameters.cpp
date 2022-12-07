/*
 * ProgramParameters.cpp
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#include "ProgramParameters.h"
#include "DirectSumParticleContainer.h"
#include "LinkedCellParticleContainer.h"
#include "ParticleCell.h"
#include "spdlog/spdlog.h"

#include <iostream>
#include <cmath>

ProgramParameters::ProgramParameters()
{   
    //std::array<double, 3> domain = {60, 30, 1};
    //BoundaryCondition b = BoundaryCondition::Outflow;
    //std::array<BoundaryCondition, 6> boundaries = {b,b,b,b,b,b};
    _particleContainer.reset(new DirectSumParticleContainer());//LinkedCellParticleContainer(std::pow(2, 1.0/6), 3.0, domain, boundaries));
    _inputFacade = std::make_unique<InputFacade>();
    _end_time = 100;
    _delta_t = 0.014;
    _sigma = 1;
    _epsilon = 5;
    _benchmark_iterations = 0;
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
    _inputFacade->readInput(*_particleContainer, filename);
}

const void ProgramParameters::resetParameters()
{
    _particleContainer->resetParticles();
}

const void ProgramParameters::setEndTime(double end_time) { _end_time = end_time; }
const void ProgramParameters::setDeltaT(double delta_t) { _delta_t = delta_t; }
const void ProgramParameters::setBenchmarkIterations(int iterations) { _benchmark_iterations = iterations; }
const void ProgramParameters::setSigma(double sigma) { _sigma = sigma; }
const void ProgramParameters::setEpsilon(double epsilon) { _epsilon = epsilon; }
const void ProgramParameters::setShowMenu(bool show_menu) { _showMenu = show_menu; }
const int ProgramParameters::getBenchmarkIterations() const { return _benchmark_iterations; }
std::shared_ptr<ParticleContainer> ProgramParameters::getParticleContainer() { return _particleContainer; }
const double ProgramParameters::getEndTime() const { return _end_time; }
const double ProgramParameters::getDeltaT() const { return _delta_t; }
const double ProgramParameters::getSigma() const { return _sigma; }
const double ProgramParameters::getEpsilon() const { return _epsilon; }
const bool ProgramParameters::getShowMenu() const { return _showMenu; }
