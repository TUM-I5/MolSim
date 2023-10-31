#pragma once

#include <iomanip>
#include <iostream>

/**
 * @brief Prints a progress bar to the console.
 *
 * @param percentage
 *
 * The bar is 100 characters wide and is filled according to the given percentage. Uses carriage return to overwrite the previous bar.
 */
void printProgress(int percentage) {
    std::cout << "\r[" << std::string(percentage, '#') << std::string(100 - percentage, ' ') << "] " << std::setw(3) << percentage << "%" << std::flush;
}
