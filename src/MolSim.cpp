
#include <boost/program_options.hpp>
#include <iostream>

#include "integration/VerletFunctor.h"
#include "simulation/Simulation.h"
#include "types/ParticleContainer.h"

int main(int argc, char* argsv[]) {
    std::string inputFilepath;
    double endTime;
    double deltaT;

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")(
        "input_file_path,f", boost::program_options::value<std::string>(&inputFilepath),
        "The path to the input file. Must be specified, otherwise the program will terminate. Can be inserted as "
        "positional argument.")("delta_t,d", boost::program_options::value<double>(&deltaT)->default_value(0.014),
                                "The time step per simulation iteration")(
        "end_time,e", boost::program_options::value<double>(&endTime)->default_value(1000),
        "The time, at which the simulation will end");

    boost::program_options::positional_options_description p;
    p.add("input_file_path", -1);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argsv).options(desc).positional(p).run(), vm);
    boost::program_options::notify(vm);

    if (argc <= 1 || vm.count("help")) {
        std::cout << desc << std::endl;
        return -1;
    }

    if (!vm.count("input_file_path")) {
        std::cout << "Error: no input file path given." << std::endl;
        std::cout << desc << std::endl;
        return -1;
    }

    Simulation simulation{inputFilepath, VerletFunctor(), deltaT, endTime};

    simulation.runSimulation();
    return 0;
}
