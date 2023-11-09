
#include <boost/program_options.hpp>
#include <iostream>

#include "integration/VerletFunctor.h"
#include "simulation/Simulation.h"
#include "types/ParticleContainer.h"

int main(int argc, char* argsv[]) {
    std::string input_file_path;
    std::string output_dir_path;
    double end_time;
    double delta_t;

    boost::program_options::options_description options_desc("Allowed options");
    options_desc.add_options()("help,h", "produce help message");
    options_desc.add_options()("input_file_path,f", boost::program_options::value<std::string>(&input_file_path),
                               "The path to the input file. Must be specified, otherwise the program will terminate. Can be inserted as positional argument.");
    options_desc.add_options()("output_dir_path,o", boost::program_options::value<std::string>(&output_dir_path)->default_value("."),
                               "The path to the directory in which to save the simulation output files. If not specified, the output will be saved in the programs working directory."
                               "Note: If the directory at the specified path does not exist, it will be created.");
    options_desc.add_options()("delta_t,d", boost::program_options::value<double>(&delta_t)->default_value(0.014),
                               "The time step per simulation iteration");
    options_desc.add_options()("end_time,e", boost::program_options::value<double>(&end_time)->default_value(1000),
                               "The time, at which the simulation will end");

    boost::program_options::positional_options_description positional_options_desc;
    positional_options_desc.add("input_file_path", -1);

    boost::program_options::variables_map variables_map;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argsv).options(options_desc).positional(positional_options_desc).run(), variables_map);
    boost::program_options::notify(variables_map);

    if (argc <= 1 || variables_map.count("help")) {
        std::cout << options_desc << std::endl;
        exit(-1);
    }
    if (!variables_map.count("input_file_path")) {
        std::cout << "Error: no input file path given." << std::endl;
        std::cout << options_desc << std::endl;
        exit(-1);
    }
    if (delta_t <= 0) {
        std::cout << "Error: timestep delta_t must be greater than 0." << std::endl;
        exit(-1);
    }
    if (end_time <= 0) {
        std::cout << "Error: end time must be greater than 0." << std::endl;
        exit(-1);
    }

    std::cout << "Configuring simulation with the following parameters:" << std::endl;
    std::cout << "Input file path: " << input_file_path << std::endl;
    std::cout << "Output directory path: " << output_dir_path << std::endl;
    std::cout << "Delta t: " << delta_t << std::endl;
    std::cout << "End time: " << end_time << std::endl;

    Simulation simulation{input_file_path, output_dir_path, Simulation::IntegrationMethod::VERLET, delta_t, end_time};

    simulation.runSimulation();
    return 0;
}
