#pragma once

#include <string>

/**
 * @brief Contains all parameters needed to run a simulation.
 *
 * This class is used to pass the parameters from the input file / CLI to the Simulation class.
 */
class SimulationParams {
   public:
    /**
     * @brief Path to the input file
     */
    std::string input_file_path;

    /**
     * @brief Path to the output directory
     */
    std::string output_dir_path;

    /**
     * @brief Time step per iteration. This specifies the accuracy of the simulation
     */
    double delta_t;

    /**
     * @brief End time of the simulation. Upon reaching this simulation time, the simulation will stop
     */
    double end_time;

    /**
     * @brief Frames per second at which to save the simulation. This is used to calculate how often to save the simulation data. If fps is
     * 0, no simulation data will be saved, and no intermediate output will be printed.
     */
    int fps;

    /**
     * @brief Length of the simulation video in seconds. This is used to calculate how often to save the simulation data. If video_length is
     * 0, no simulation data will be saved, and no intermediate output will be printed.
     */
    int video_length;

    /**
     * @brief Log level to use for the logger. Possible values are "trace", "debug", "info", "warn", "err", "critical", "off"
     */
    std::string log_level;

    SimulationParams(const std::string& input_file_path, const std::string& output_dir_path, double delta_t, double end_time, int fps,
                     int video_length, const std::string& log_level)
        : input_file_path(input_file_path),
          output_dir_path(output_dir_path),
          delta_t(delta_t),
          end_time(end_time),
          fps(fps),
          video_length(video_length),
          log_level(log_level) {}

    SimulationParams() {}
};