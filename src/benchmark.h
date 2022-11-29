//
// Created by alex on 26.11.2022.
//

#pragma once

#include "io/input/cli/CLIArgs.h"

int runBenchmark(double dt, double et, double st, double sig, double eps, std::vector<std::string>& inputFiles, const std::string& bCond, bool linkedCell);

