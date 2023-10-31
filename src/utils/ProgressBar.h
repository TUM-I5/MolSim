#pragma once

#include <iomanip>
#include <iostream>

/**
 * @brief Prints a progress bar to the console.
 *
 * @param percentage
 */
void printProgress(int percentage) {
    std::cout << "\r[" << std::string(percentage, '#') << std::string(100 - percentage, ' ') << "] " << std::setw(3) << percentage << "%" << std::flush;
}
