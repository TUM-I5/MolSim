
#include "inputHandling/FileReader.h"
#include "particleModel/ParticleContainer.h"
#include "inputHandling/CuboidGenerator.h"
#include "Simulation.h"




#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char *argsv[])
{
    //initialize default values
    double end_time = 5;
    double delta_t = 0.0002;
    spdlog::level::level_enum logging_level = spdlog::level::debug ;

    std::string filename;
    ParticleContainer particleContainer;
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
               "\n"
               "Returns:\n"
               "                  several .vtu files that can be used for visualisation in Paraview\n";

    std::cout << "Hello from MolSim for PSE!" << std::endl;
    ///variables for the argument parsing
    int opt;
    std::string log_mode;
    while ((opt = getopt(argc, argsv, "t:e:f:l:h")) != -1)
    {
        switch (opt)
        {
            case 't':
                try
                {
                    delta_t = std::stod(optarg);
                    std::cout << "delta_t: " << delta_t << std::endl;
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << "Invalid argument for delta_t" << e.what() << std::endl;
                }
                catch (const std::out_of_range &e)
                {
                    std::cerr << "The delta_t is Out of range" << e.what() << std::endl;
                }
                break;
            case 'e':
                try
                {
                    end_time = std::stod(optarg);
                    std::cout << "end_time: " << end_time << std::endl;
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << "Invalid argument for the endtime" << e.what() << std::endl;
                }
                catch (const std::out_of_range &e)
                {
                    std::cerr << "The endtime is Out of Range" << e.what() << std::endl;
                }

                break;
            case 'l':
                log_mode = std::string(optarg); 
                if(log_mode=="off"){
                    logging_level = spdlog::level::off;
                }else if(log_mode=="trace"){
                    logging_level = spdlog::level::trace;
                }else if(log_mode=="debug"){
                    logging_level = spdlog::level::debug;
                }else if(log_mode=="info"){
                    logging_level = spdlog::level::info;
                }else if(log_mode=="error"){
                    logging_level = spdlog::level::err;
                }else if(log_mode=="critical"){
                    logging_level = spdlog::level::critical;
                }
                break;
            case 'f':
                filename = std::string(optarg);
                break;
            case 'h':
                std::cout << msg;
                return 0;
            default:
                std::cerr << msg;
                return 1;
        }
    }

    FileReader::filelog->set_level(logging_level);
    Simulation::simulation_log->set_level(logging_level);
    spdlog::set_level(spdlog::level::debug);


    auto cuboids = fileReader.readCuboidFile(filename);


    addCuboids(particleContainer,cuboids);
    

    Simulation::runSimulation(particleContainer, end_time, delta_t);

    return 0;
}
