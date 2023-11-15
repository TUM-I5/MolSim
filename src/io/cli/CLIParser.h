#pragma once

#include <iostream>
#include <string>

/**
 * @brief Parses the command line arguments
 *
 * Parses the command line arguments and returns a tuple containing all the arguments.
 */
std::tuple<std::string, std::string, double, double, int, int, std::string> parse_arguments(int argc, char* argsv[]);
