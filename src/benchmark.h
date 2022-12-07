//
// Created by alex on 26.11.2022.
//

#pragma once

#include "io/input/cli/CLIArgs.h"
#include "io/input/Configuration.h"
#include "io/input/sim_input/types.h"

using namespace io::input;

int runBenchmark(io::input::Configuration& config, std::vector<std::string>& files, io::input::type t);

