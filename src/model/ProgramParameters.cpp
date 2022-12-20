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
    domain = {180, 90, 1};
    BoundaryCondition o = BoundaryCondition::Outflow;
    boundaries = {o, o, o, o, o, o};
    end_time = 100;
    delta_t = 10;
    sigma = 1;
    cutoff = 3;
    writeFrequency = 10;
    particleContainer.reset(new LinkedCellParticleContainer(sigma, cutoff, domain, boundaries));
    baseName = "outputVTK";
    temp_init = 40;
    brownianMotion = true;
    n_thermostats = 1000;
    temp_target = temp_init;
    delta_temp = 0.0005;
    g_grav = -12.44;
    benchmark_iterations = 0;
    showMenu = false;
    createCheckpoint = false; 
    memoryLogger = spdlog::get("memory_logger");
    memoryLogger->info("ProgramParameters generated!");
}

ProgramParameters::~ProgramParameters()
{
    memoryLogger->info("ProgramParameters destructed!");
}

const void ProgramParameters::resetParameters()
{
    particleContainer->resetParticles();
}

const void ProgramParameters::setEndTime(double end_time) { this->end_time = end_time; }
const void ProgramParameters::setDeltaT(double delta_t) { this->delta_t = delta_t; }
const void ProgramParameters::setBenchmarkIterations(int iterations) { this->benchmark_iterations = iterations; }
const void ProgramParameters::setSigma(double sigma)
{
    this->sigma = sigma;
    if (typeid(*particleContainer) == typeid(LinkedCellParticleContainer))
    {
        particleContainer.reset(new LinkedCellParticleContainer(sigma, cutoff, domain, boundaries));
    }
}
const void ProgramParameters::setCutoff(double cutoff)
{
    this->cutoff = cutoff;
    if (typeid(*particleContainer) == typeid(LinkedCellParticleContainer))
    {
        particleContainer.reset(new LinkedCellParticleContainer(sigma, cutoff, domain, boundaries));
    }
}
const void ProgramParameters::setDomain(std::array<double, 3> domain)
{
    this->domain = domain;
    if (typeid(*particleContainer) == typeid(LinkedCellParticleContainer))
    {
        particleContainer.reset(new LinkedCellParticleContainer(sigma, cutoff, domain, boundaries));
    }
}
const void ProgramParameters::setBoundaries(std::array<BoundaryCondition, 6> boundaries)
{
    this->boundaries = boundaries;
    
    //if 2D overwrite z-boundaries to be outflow
    if (this->domain[2] == 1) {
        this->boundaries[4] = BoundaryCondition::Outflow;
        this->boundaries[5] = BoundaryCondition::Outflow;
    }
    //error if periodic bondaries are not on opposite sides
    BoundaryCondition p = BoundaryCondition::Periodic;
    if ((boundaries[0] == p) != (boundaries[1] == p) 
        || (boundaries[2] == p) != (boundaries[3] == p)
        || (boundaries[4] == p) != (boundaries[5] == p))
    {
        throw std::invalid_argument("Periodic boundaries have to be on opposite sides");
    }
        

    if (typeid(*particleContainer) == typeid(LinkedCellParticleContainer))
    {
        particleContainer.reset(new LinkedCellParticleContainer(sigma, cutoff, domain, boundaries));
    }
}
const void ProgramParameters::setWriteFrequency(int writeFrequency) { this->writeFrequency = writeFrequency; }
const void ProgramParameters::setBaseName(std::string baseName) { this->baseName = baseName; }
const void ProgramParameters::setTempInit(double temp_init) { this->temp_init = temp_init; }
const void ProgramParameters::setBrownianMotion(bool brownianMotion) { this->brownianMotion = brownianMotion; }
const void ProgramParameters::setNThermostats(int n_thermostats) { this->n_thermostats = n_thermostats; }
const void ProgramParameters::setTempTarget(double temp_target) { this->temp_target = temp_target; }
const void ProgramParameters::setDeltaTemp(double delta_temp) { this->delta_temp = delta_temp; }
const void ProgramParameters::setGGrav(double g_grav) { this->g_grav = g_grav; }
const void ProgramParameters::setShowMenu(bool show_menu) { this->showMenu = show_menu; }
const void ProgramParameters::setCreateCheckpoint(bool createCheckpoint) { this->createCheckpoint = createCheckpoint; }
const int ProgramParameters::getBenchmarkIterations() const { return benchmark_iterations; }
std::shared_ptr<ParticleContainer> ProgramParameters::getParticleContainer() { return particleContainer; }
const double ProgramParameters::getEndTime() const { return end_time; }
const double ProgramParameters::getDeltaT() const { return delta_t; }
const double ProgramParameters::getSigma() const { return sigma; }
const double ProgramParameters::getCutoff() const { return cutoff; }
const std::array<double, 3> ProgramParameters::getDomain() const { return domain; }
const std::array<BoundaryCondition, 6> ProgramParameters::getBoundaries() const { return boundaries; }
const int ProgramParameters::getWriteFrequency() { return writeFrequency; }
const double ProgramParameters::getTempInit() const { return temp_init; }
const bool ProgramParameters::getBrownianMotion() const { return brownianMotion; }
const int ProgramParameters::getNThermostats() const { return n_thermostats; }
const double ProgramParameters::getTempTarget() const { return temp_target; }
const double ProgramParameters::getDeltaTemp() const { return delta_temp; }
const double ProgramParameters::getGGrav() const { return g_grav; }
const std::string ProgramParameters::getBaseName() { return baseName; }
const bool ProgramParameters::getShowMenu() const { return showMenu; }
const bool ProgramParameters::getCreateCheckpoint() { return createCheckpoint; }
