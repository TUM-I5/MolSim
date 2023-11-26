#pragma once

#include <string>

/**
 * @brief Contains all parameters needed to run a simulation.
 *
 * This class is used to pass the parameters from the input file / CLI to the Simulation class.
 */
class SimulationParams {
   public:
    std::string input_file_path;
    std::string output_dir_path;

    double delta_t;
    double end_time;

    int fps;
    int video_length;

    std::string log_level;

    int container_type;
    double domain_size;
    double cutoff_radius;

    SimulationParams(const std::string& input_file_path, const std::string& output_dir_path, double delta_t, double end_time, int fps,
                     int video_length, const std::string& log_level, int container_type, double domain_size, double cutoff_radius);

    SimulationParams() {}
};