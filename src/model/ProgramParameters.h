/*
 * ProgramParameters.h
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#pragma once

#include "../model/ParticleContainer.h"
#include "../model/DirectSumParticleContainer.h"
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
    double epsilon;                                       /// epsilon parameter for Lennard-Jones potential
    double cutoff;                                        /// cutoff for the linked cell algorith
    std::array<int, 3> domain;                            /// the size of the domain
    int benchmark_iterations;                             /// number of runs in benchmark mode, 0 for normal simulations
    bool showMenu;                                        /// true if menu should be shown, false otherwise
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

    const void setEpsilon(double epsilon);

    const void setCutoff(double cuttoff);

    const void setDomain(std::array<int, 3> domain);

    const void setShowMenu(bool show_menu);

    std::shared_ptr<ParticleContainer> getParticleContainer();

    const double getEndTime() const;

    const double getDeltaT() const;

    const int getBenchmarkIterations() const;

    const double getSigma() const;

    const double getEpsilon() const;

    const double getCutoff() const;

    const std::array<int, 3> getDomain() const;

    const bool getShowMenu() const;
};