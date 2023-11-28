#pragma once

#include <string>
#include <tuple>
#include <variant>

#include "io/output/FileOutputHandler.h"

/**
 * @brief Contains all parameters needed to run a simulation.
 *
 * This class is used to pass the parameters from the input file / CLI to the Simulation class.
 */
class SimulationParams {
   public:
    struct DirectSumType {};

    struct LinkedCellsType {
        std::array<double, 3> domain_size;
        double cutoff_radius;

        LinkedCellsType() = delete;
        LinkedCellsType(const std::array<double, 3>& domain_size, double cutoff_radius)
            : domain_size(domain_size), cutoff_radius(cutoff_radius) {}
    };

    std::string input_file_path;
    std::string output_dir_path;

    double delta_t;
    double end_time;

    int fps;
    int video_length;

    std::variant<DirectSumType, LinkedCellsType> container_type;

    FileOutputHandler::OutputFormat output_format;

    SimulationParams(const std::string& input_file_path, const std::string& output_dir_path, double delta_t, double end_time, int fps,
                     int video_length, const std::variant<DirectSumType, LinkedCellsType>& container_type,
                     const std::string& output_format);

    SimulationParams() = delete;
};