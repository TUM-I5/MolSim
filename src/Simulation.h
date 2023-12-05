#pragma once


#include "particleModel/storage/SimulationContainer.h"
#include "particleModel/storage/CellContainer.h"
#include "particleModel/updating/CellCalculator.h"
#include "particleModel/storage/ParticleContainer.h"
#include "particleModel/updating/Model.h"
#include "outputWriter/XYZWriter.h"
#include <variant>


/**
 * @brief start of the particle simulation
 *
 * repeatedly iterates over particles inside particleContainer until
 * end_time is reached with delta_t steps for each iteration
 *
 * @param particleContainer contains all particles to simulate
 * @param end_time timespan to simulate
 * @param delta_t time step for each iteration
 * @param performance_measurement bool to set the performance measuring of the simulation
 * @param calculate int to set the performance measuring of the simulation
 */

void runSimulation(SimulationContainer &particleContainer, std::variant<Model,CellCalculator> calculate,
                         const double end_time,const double delta_t, const size_t write_frequency, bool performance_measurement);
    /**
    * @brief plot the particles to a xyz-file
    */
    void plotParticles(ParticleContainer &particleContainer, int iteration);





