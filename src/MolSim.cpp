#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "inputHandling/FileReader.h"
#include "inputHandling/CuboidGeneration.h"

#include "particleModel/storage/ParticleContainer.h"
#include "particleModel/storage/CellContainer.h"

#include "particleModel/updating/Calculator.h"
#include "particleModel/updating/CellCalculator.h"


#include "Simulation.h"
#include "utils/ForceCalculations.h"


#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <iostream>
#include <string>
#include <unistd.h>

/**
 * @brief Parsing the program arguments and starting the Simulation
*/
int main(int argc, char *argsv[])
{
    //initialize default values
    double end_time = 5;
    double delta_t = 0.0002;
    bool performance_measurement = false;
    spdlog::level::level_enum logging_level = spdlog::level::info;

    std::string filename;
    FileReader fileReader;

    auto msg = "Usage ./MolSim [-e<double>] [-t<double>] [-l<String>] -f<String>\n"
               " -e<double>:        gives the end_time of the simulation\n"
               " -t<double>:        gives the step size used for the simulation\n"
               " -f<String>:        gives the filename from which the initial state\n"
               "                    of the Particles is read, these are the particles\n"
               "                    that will get simulated\n"
               " -l<String>:        specifies the level of logging, e.g. how fine grained programm logs are.\n"
               "                    can either be \"off\" \"trace\", \"debug\", \"info\", \"error\" or \"critical\".\n"
               "                    The default level is \"debug\".\n"
               " -h                 prints a help message\n"
               " -p                 if the flag is set, the programm will measure the time for the execution.\n"
               "                    therefore no vtk output and no logging will happen (specifing a log level at\n"
               "                    the same time is undefined behaviour)\n"
               "\n"
               "Returns:\n"
               "                  several .vtu files that can be used for visualisation in Paraview\n";



    ///variables for the argument parsing
    int opt;
    std::string log_mode;
    while ((opt = getopt(argc, argsv, "t:e:f:l:hp")) != -1)
    {
        switch (opt)
        {
            case 't':
                try
                {
                    delta_t = std::stod(optarg);
                    std::cout << ("delta_t: " + std::to_string(delta_t)) << std::endl;
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << ("Invalid argument for delta_t" + std::string(e.what())) << std::endl; 
                }
                catch (const std::out_of_range &e)
                {
                    std::cerr << ("The delta_t is Out of range" + std::string(e.what())) << std::endl; 
                }
                break;
            case 'e':
                try
                {
                    end_time = std::stod(optarg);
                    SPDLOG_INFO("end_time: " + std::to_string(end_time));
                }
                catch (const std::invalid_argument &e)
                {
                    SPDLOG_ERROR("Invalid argument for the endtime" + std::string(e.what()));
                }
                catch (const std::out_of_range &e)
                {
                    SPDLOG_ERROR("The endtime is Out of Range" + std::string(e.what()));
                }

                break;
            case 'l':
                log_mode = std::string(optarg); 
                if(log_mode=="off"){
                    logging_level = spdlog::level::off;
                }else if(log_mode=="trace"){
                    logging_level = spdlog::level::trace;   //enables current time logging
                }else if(log_mode=="debug"){
                    logging_level = spdlog::level::debug;   //enables logs.txt writing
                }else if(log_mode=="info"){
                    logging_level = spdlog::level::info;    //enables progress logging
                }else if(log_mode=="error"){
                    logging_level = spdlog::level::err;
                }else if(log_mode=="critical"){
                    logging_level = spdlog::level::critical;
                }
                break;
            case 'f':
                filename = std::string(optarg);
                break;
            case 'p':
                performance_measurement = true;
                logging_level = spdlog::level::off;
                break;
            case 'h':
                std::cout << msg;
                return 0;
            default:
                std::cerr << msg;
                return 1;
        }
    }

    auto logger = spdlog::basic_logger_mt("logger", "logs.txt");
    spdlog::set_level(logging_level);
    SPDLOG_INFO("Hello from MolSim for PSE!");

    CellContainer cellContainer(4,4,4,2.0,1.0);

    cellContainer.addParticle({2, 2, 2}, {0, 0, 0}, 1);
    cellContainer.addParticle({2, 2, 3}, {0, 0, 0}, 1);
    cellContainer.addParticle({2, 3, 2}, {0, 0, 0}, 1);
    cellContainer.addParticle({2, 3, 3}, {0, 0, 0}, 1);
    cellContainer.addParticle({3, 2, 2}, {0, 0, 0}, 1);
    cellContainer.addParticle({3, 2, 3}, {0, 0, 0}, 1);
    cellContainer.addParticle({3, 3, 2}, {0, 0, 0}, 1);
    cellContainer.addParticle({3, 3, 3}, {0, 0, 0}, 1);

    std::cout << cellContainer.to_string() << std::endl;


    auto iter = cellContainer.begin_boundary(); 
    while(iter != cellContainer.end_boundary()){
        std::cout << "Got Index: [" << iter.x << ", " << iter.y << " ," << iter.z << "] \n";
        ++iter;
        std::cout << "Got Index: [" << iter.x << ", " << iter.y << " ," << iter.z << "] \n";
    } 




    return 0;

    //parse Program arguments
    //auto args = fileReader.readProgramArguments(filename);

    //function for Forces between two particles
    ForceCalculation force_func = forceLennJonesPotentialFunction(1.0,5.0);


    /*In the following two examples how to start the simulation(old/new one)*/


    //initalize old simulation
    ParticleContainer particleContainer;
    Model model(particleContainer, delta_t, force_func);
    runSimulation(particleContainer,model, end_time, delta_t,performance_measurement);
    

    //initalize new simulation
    //CellContainer cellContainer(1,1,1,1,1);
    CellCalculator CellCalculator(cellContainer,delta_t,force_func);
    runSimulation(cellContainer,CellCalculator,end_time,delta_t,performance_measurement);

    // auto cuboids = fileReader.readCuboidFile(filename);


    // addCuboids(particleContainer,cuboids);
    


    return 0;
}
