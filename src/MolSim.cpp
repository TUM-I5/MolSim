#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "inputHandling/FileReader.h"
#include "inputHandling/CuboidGeneration.h"
#include "inputHandling/SphereGeneration.h"

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
    bool old = false;
    spdlog::level::level_enum logging_level = spdlog::level::info;

    std::string filename;
    FileReader fileReader;

    auto msg = "Usage ./MolSim [-e<double>] [-t<double>] [-l<String>] [-p] [-o] -f<String>\n"
               " Info:              The cmd line arguments -e and -t are deprecated and will be ignored.\n"
               "                    See the /input folder for the parameters.xsd schema, in which program arguments should be specified\n"
               " -e<double>:        gives the end_time of the simulation\n"
               " -t<double>:        gives the step size used for the simulation\n"
               " -f<String>:        gives the filename of an .xml file, that has to follow\n"
               "                    the xsd schema defined in input/parameters.xsd.\n"
               "                    from this file all programm arguments / options will be read(see README)\n"
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

    std::cout <<  "Hello from MolSim for PSE!" << std::endl;

    ///variables for the argument parsing
    int opt;
    std::string log_mode;
    while ((opt = getopt(argc, argsv, "t:e:f:l:hpo")) != -1)
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
                    std::cout << "end_time: " + std::to_string(end_time) << std::endl;
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << ("Invalid argument for the endtime" + std::string(e.what())) << std::endl;                    
                }
                catch (const std::out_of_range &e)
                {
                    std::cerr << ("The endtime is Out of Range" + std::string(e.what())) << std::endl;                    
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
            case 'o':
                old = true;
            case 'h':
                std::cout << msg;
                return 0;
            default:
                std::cerr << msg;
                return 1;
        }
    }

    //Particle p1({0.5,})
    //Particle p2({0.5,})


    //return 0;

    auto logger = spdlog::basic_logger_mt("logger", "logs.txt");
    spdlog::set_level(logging_level);

    FileReader::ProgramArgs args = fileReader.readProgramArguments(filename);
    SPDLOG_INFO("Read:\n" + args.to_string());

    if(!old){
    CellContainer cellContainer(args.domain_dimensions[0],args.domain_dimensions[1],args.domain_dimensions[2],args.cut_of_radius,args.cell_size);
    CellCalculator cellCalculator(cellContainer,args.delta_t,"LennJones");

    addCuboids(cellContainer,args.cuboids);
    addSpheres(cellContainer,args.spheres,2);

    cellContainer.createPointers();

    SPDLOG_INFO("Starting the Simulation with new version:");
    runSimulation(cellContainer,cellCalculator,args.t_end,args.delta_t,performance_measurement);

    }else{


    //config for old program simulation
    //auto cuboids = fileReader.readCuboidFile(filename);
    ParticleContainer particleContainer;
    addCuboids(particleContainer,args.cuboids);
    addSpheres(particleContainer,args.spheres,2);
    Model model(particleContainer, "LennJones", args.delta_t);

    SPDLOG_INFO("Starting the Simulation with old version:");
    runSimulation(particleContainer,model, args.t_end, args.delta_t,performance_measurement);
    }
 
}
