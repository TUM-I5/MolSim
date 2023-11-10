#pragma once

#include <iomanip>
#include <iostream>

/**
 * @brief Prints a progress bar to the console.
 * @param percentage
 * @param estimated_time
 *
 * The bar is 100 characters wide and is filled according to the given percentage. Uses carriage return to overwrite the previous bar.
 * Example: [##########          ]  50% (eta 00:12:34)
 * If estimated_time is positive, the estimated remaining time is printed in the format (eta hh:mm:ss) otherwise it is omitted.
 */
void printProgress(int percentage, int estimated_time) {
    std::cout << "\r[\033[32m"
              << std::string(percentage, '#') << "\033[0m"
              << std::string(100 - percentage, '-') << "] "
              << std::setw(3) << percentage
              << "%";

    if (estimated_time > 0) {
        int hours = estimated_time / 3600;
        int minutes = (estimated_time % 3600) / 60;
        int seconds = estimated_time % 60;

        std::cout << " (eta: "
                  << std::setfill('0')
                  << std::setw(2) << hours << ":"
                  << std::setw(2) << minutes << ":"
                  << std::setw(2) << seconds << ")"
                  << std::setfill(' ');
    }

    std::cout << std::flush;

    if (percentage == 100) {
        std::cout << std::endl;
    }
}