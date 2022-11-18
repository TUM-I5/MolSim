
#include "./inputReader/FileReader.h"
#include "./model/ParticleContainer.h"
#include "./model/ProgramParameters.h"
#include "./simulation/Simulation.h"
#include "ConsoleMenu.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <iostream>
#include <list>
#include <set>
#include <memory>
#include <unistd.h>
#include <sstream>
#include <algorithm>

void printHelp();

const void handleInput(int argc, char *argsv[]);

const void initializeLoggers(spdlog::level::level_enum level);

const void handleLogging(int argc, char *argsv[]);

ProgramParameters *programParameters;

int main(int argc, char *argsv[])
{
    // handleLogging is called first, because the spdlog level has to be set when initializing
    handleLogging(argc, argsv);

    programParameters = new ProgramParameters();

    handleInput(argc, argsv);

    if (programParameters->getShowMenu())
    {
        std::unique_ptr<ConsoleMenu> consoleMenu = std::make_unique<ConsoleMenu>(ConsoleMenu(programParameters));
        consoleMenu->openMenu();
    }
    else
    {
        std::unique_ptr<Simulation> simulation = std::make_unique<Simulation>(Simulation(programParameters));
        simulation->simulate();
    }
    spdlog::shutdown();

    delete (programParameters);
    return EXIT_SUCCESS;
}

const void handleLogging(int argc, char *argsv[])
{
    spdlog::level::level_enum level = spdlog::level::info;
    while (1)
    {
        int result = getopt(argc, argsv, "mht:f:d:l:");
        if (result == -1)
        {
            break;
        }
        if (result == 'l')
        {
            if (Input::isInt(optarg) && Input::isValidLevel(optarg))
            {
                int new_level = std::__cxx11::stoi(optarg);
                if (new_level == 0)
                {
                    level = spdlog::level::off;
                }
            }
            else
            {
                std::cout << "Error: Please specify a valid log level" << std::endl;
                printHelp();
                exit(0);
            }
            break;
        }
    }
    optind = 1;
    initializeLoggers(level);
}

const void handleInput(int argc, char *argsv[])
{
    while (1)
    {
        int result = getopt(argc, argsv, "mht:f:d:l:");

        if (result == -1)
        {
            break;
        }
        switch (result)
        {
        case '?':
            printHelp();
            exit(1);
        case ':':
            printHelp();
            exit(1);
        case 'h':
            printHelp();
            exit(0);
        case 'm':
            programParameters->setShowMenu(true);
            break;
        case 't':
            if (Input::isDouble(optarg))
            {
                programParameters->setEndTime(std::__cxx11::stod(optarg));
            }
            else
            {
                std::cout << "Error: end_time parameter (-t) is not a double" << std::endl;
                printHelp();
                exit(0);
            }
            break;
        case 'd':
            if (Input::isDouble(optarg))
            {
                programParameters->setDeltaT(std::__cxx11::stod(optarg));
            }
            else
            {
                std::cout << "Error: delta_t parameter (-d) is not a double" << std::endl;
                printHelp();
                exit(0);
            }
            break;
        case 'f':
        {
            std::ifstream test(optarg);
            if (!test)
            {
                std::cout << "Error: please specify a valid filename" << std::endl;
                printHelp();
                exit(0);
            }
            else
            {
                programParameters->readFromFile(optarg);
            }
        }
        break;
        default:
            break;
        }
    }
}

const void initializeLoggers(spdlog::level::level_enum level)
{
    try
    {
        auto simulation_logger = spdlog::basic_logger_mt("simulation_logger", "../logs/simulation.txt", true);
        simulation_logger->set_level(level);
        auto input_logger = spdlog::basic_logger_mt("input_logger", "../logs/input.txt", true);
        input_logger->set_level(level);
        auto output_logger = spdlog::basic_logger_mt("output_logger", "../logs/output.txt", true);
        output_logger->set_level(level);
        auto memory_logger = spdlog::basic_logger_mt("memory_logger", "../logs/memory.text", true);
        memory_logger->set_level(level);
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}

void printHelp()
{
    printf(" -f <filename> .......... The path to an input file. If not specified and no cuboids are generated, no particles appear in the simulation.\n");
    printf(" -t <end_time> .......... The end time of the simulation. If not specified, 100 is used\n");
    printf(" -d <delta_t> ........... The size of the time steps in the simulation. If not specified 0.014 is used\n");
    printf(" -l <log_level>.......... Activate '1' or deactivate '0' the loggers. If not specified, the loggers are activated\n");
    printf(" -m ..................... Enter the console menu, here you can read in files, create cuboids and re-run the program with the same parameters\n");
    printf(" -h ..................... Help\n");
}
