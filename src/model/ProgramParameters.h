/*
 * ProgramParameters.h
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#pragma once

#include "../model/ParticleContainer.h"
#include "../model/DirectSumParticleContainer.h"
#include "../model/ParticleCell.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

#include <list>

class InputFacade;

/**
 * @brief wrapper for all program parameters. Makes it easy to safe and rerun with the same parameters
 */
class ProgramParameters
{
private:
    std::shared_ptr<ParticleContainer> particleContainer; /// container for all the particles
    double end_time;                                      /// end_time of the simulation
    double delta_t;                                       /// increase in step size for the time
    double sigma;                                         /// sigma parameter for Lennard-Jones potential
    double cutoff;                                        /// cutoff for the linked cell algorith
    std::array<double, 3> domain;                         /// the size of the domain
    std::array<BoundaryCondition, 6> boundaries;          /// the boundaries for the simulation
    int writeFrequency;                                   /// the number of iterations after which an vtk file is written
    std::string baseName;                                 /// the path to the output folder
    double temp_init;                                     /// the initial temperature
    bool brownianMotion;                                  /// specifies if particles should be initialized with brownian motion
    int n_thermostats;                                    /// the number of iterations after which the thermostat is applied
    double temp_target;                                   /// the target temperature of the simulation
    double delta_temp;                                    /// the maximum increase in the temperature per iteration
    double g_grav;                                        /// the gravitational constant for the simulation
    int benchmark_iterations;                             /// number of runs in benchmark mode, 0 for normal simulations
    bool showMenu;                                        /// true if menu should be shown, false otherwise
    bool createCheckpoint;                                /// true if a checkpoint should be created, false otherwise
    std::shared_ptr<spdlog::logger> memoryLogger;         /// a speedlog logger which logs construction and destruction of particles

public:
    /**
     * @brief constructor for the ProgramParameters, initialises all of the parameters of the class
     */
    ProgramParameters();
    ~ProgramParameters();

    /**
     * @brief runs the simulation with the parameters that are currently set
     */
    const void runWithCurrentParameters();

    /**
     * @brief removes all particles from the simulation
     */
    const void resetParameters();

    /**
     * Getters/Setters
     */
    const void setEndTime(double end_time);

    const void setDeltaT(double delta_t);

    const void setBenchmarkIterations(int iterations);

    const void setSigma(double sigma);

    const void setCutoff(double cuttoff);

    const void setDomain(std::array<double, 3> domain);

    const void setBoundaries(std::array<BoundaryCondition, 6> boundaries);

    const void setWriteFrequency(int writeFrequency);

    const void setBaseName(std::string baseName);

    const void setTempInit(double temp_init);

    const void setBrownianMotion(bool browninanMotion);

    const void setNThermostats(int n_thermostats);

    const void setTempTarget(double temp_target);

    const void setDeltaTemp(double delta_temp);

    const void setGGrav(double g_grav);

    const void setShowMenu(bool show_menu);

    const void setCreateCheckpoint(bool createCheckpoint);

    std::shared_ptr<ParticleContainer> getParticleContainer();

    const double getEndTime() const;

    const double getDeltaT() const;

    const int getBenchmarkIterations() const;

    const double getSigma() const;

    const double getCutoff() const;

    const std::array<double, 3> getDomain() const;

    const std::array<BoundaryCondition, 6> getBoundaries() const;

    const int getWriteFrequency();

    const std::string getBaseName();

    const double getTempInit() const;

    const bool getBrownianMotion() const;

    const int getNThermostats() const;

    const double getTempTarget() const;

    const double getDeltaTemp() const;

    const double getGGrav() const;

    const bool getShowMenu() const;

    const bool getCreateCheckpoint(); 
};