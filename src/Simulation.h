#pragma once

#include "particleModel/storage/CellContainer.h"
#include "particleModel/updating/CellCalculator.h"



/**
 * @brief start of the particle simulation
 *
 * repeatedly iterates over particles inside particleContainer until
 * end_time is reached with delta_t steps for each iteration
 *
 * \image latex runtime_nice.png "Runtime Measurements"
 *
 * @param particleContainer contains all particles to simulate
 * @param end_time timespan to simulate
 * @param delta_t time step for each iteration
 * @param performance_measurement bool to set the performance measuring of the simulation
 * @param calculator int to set the performance measuring of the simulation
 */

void runSimulation(CellContainer &particleContainer, CellCalculator calculator,
                   const double end_time,const double delta_t,
                   const size_t write_frequency,const int thermostats_frequency,
                   bool performance_measurement);

/**
* @brief plot the particles to a xyz-file
*/
void plotParticles(CellContainer &container, int iteration);





