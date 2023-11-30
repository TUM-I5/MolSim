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
    /**
     * @brief Struct to specify the type of the particle container as DirectSumType
     */
    struct DirectSumType {};

    /**
     * @brief Struct to specify the type of the particle container as LinkedCellsType (needs domain_size and cutoff_radius)
     */
    struct LinkedCellsType {
        std::array<double, 3> domain_size;
        double cutoff_radius;

        LinkedCellsType() = delete;
        LinkedCellsType(const std::array<double, 3>& domain_size, double cutoff_radius)
            : domain_size(domain_size), cutoff_radius(cutoff_radius) {}
    };

    /**
     * @brief Path to the input file of the simulation
     */
    std::string input_file_path;

    /**
     * @brief Path to the directory in which to save the simulation output
     */
    std::string output_dir_path;

    /**
     * @brief Time step of a single simulation iteration
     */
    double delta_t;

    /**
     * @brief End time of the simulation
     */
    double end_time;

    /**
     * @brief Frames per second at which to save the simulation. This is used to calculate how often to save the simulation data
     */
    int fps;

    /**
     * @brief Expected length of the simulation video in seconds. This is used to calculate how often to save the simulation data
     */
    int video_length;

    /**
     * @brief Type of the particle container
     */
    std::variant<DirectSumType, LinkedCellsType> container_type;

    /**
     * @brief Output file format of the simulation
     */
    FileOutputHandler::OutputFormat output_format;

    /**
     * @brief Construct a new SimulationParams object
     *
     * @param input_file_path Path to the input file of the simulation
     * @param output_dir_path Path to the directory in which to save the simulation output
     * @param delta_t Time step of a single simulation iteration
     * @param end_time End time of the simulation
     * @param fps Frames per second at which to save the simulation. This is used to calculate how often to save the simulation data
     * @param video_length Expected length of the simulation video in seconds. This is used to calculate how often to save the simulation
     * data
     * @param container_type Type of the particle container
     * @param output_format Output file format of the simulation
     */
    SimulationParams(const std::string& input_file_path, const std::string& output_dir_path, double delta_t, double end_time, int fps,
                     int video_length, const std::variant<DirectSumType, LinkedCellsType>& container_type,
                     const std::string& output_format);

    /**
     * @brief Dissallow default construction of a SimulationParams object (would have invalid values for a simulation)
     */
    SimulationParams() = delete;
};