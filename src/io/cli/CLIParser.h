#pragma once

#include <iostream>
#include <string>

#include "simulation/SimulationParams.h"

/**
 * @brief Parses the command line arguments
 *
 * @param argc Number of arguments
 * @param argsv Array of arguments
 * @return SimulationParams containing the parsed arguments, or default values for the arguments that were not specified
 *
 * Parses the command line arguments and returns a tuple containing all the arguments.
 */
SimulationParams parse_arguments(int argc, char* argsv[]);

/**
 * @brief Merges the simulation parameters retrieved via the command line with the ones from the XML file. Command line arguments have
 * priority.
 *
 * @param params_cli Simulation parameters retrieved via Command line arguments
 * @param params_xml Simulation parameters retrieved via XML file
 * @return SimulationParams containing the merged parameters. Command line arguments have priority.
 *
 * Merges the command line arguments with the XML file and returns a tuple containing all the arguments.
 */
SimulationParams merge_parameters(const SimulationParams& params_cli, const SimulationParams& params_xml);