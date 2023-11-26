#pragma once

#include <iostream>
#include <string>

#include "simulation/SimulationParams.h"

/**
 * @brief Parses the command line arguments
 *
 * Parses the command line arguments and returns a tuple containing all the arguments.
 */
SimulationParams parse_arguments(int argc, char* argsv[]);
SimulationParams merge_parameters(const SimulationParams& params_cli, const SimulationParams& params_xml);
