
#include "./inputReader/FileReader.h"
#include "./model/ParticleContainer.h"
#include "./model/ProgramParameters.h"
#include "./simulation/Simulation.h"
#include "ConsoleMenu.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <ctime>
#include <iostream>
#include <list>
#include <set>
#include <memory>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <chrono>

void printHelp();

const void handleInput(int argc, char *argsv[]);

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
    else if (programParameters->getMode() == Mode::Simulation)
    {
        std::unique_ptr<Simulation> simulation = std::make_unique<Simulation>(Simulation(programParameters));
        simulation->simulate();
    }
    else {
        std::cout << "MolSim Group G > Running benchmark ..." << std::endl;

        using namespace std::chrono;

        time_point<high_resolution_clock> start_point, end_point;
        auto total_time = microseconds(0).count();

        for (int i = 0; i < programParameters->getBenchmarkIterations(); i++) {
            std::unique_ptr<Simulation> simulation = std::make_unique<Simulation>(Simulation(programParameters));
            
            start_point = high_resolution_clock::now();
            simulation->simulate();
            end_point = high_resolution_clock::now();

            auto start = time_point_cast<microseconds>(start_point).time_since_epoch().count();
            auto end = time_point_cast<microseconds>(end_point).time_since_epoch().count();

            total_time += (end - start);
        }
        auto mean_time = total_time / programParameters->getBenchmarkIterations();
        std::cout << "MolSim Group G > Mean duration over " << programParameters->getBenchmarkIterations() << " run(s): " << mean_time/1000000.0 << " seconds" << std::endl;
        std::cout << "MolSim Group G > ... Finished" << std::endl;
    }
    spdlog::shutdown();

  spdlog::drop_all();
  delete (programParameters);
  return EXIT_SUCCESS;
}

const void handleLogging(int argc, char *argsv[])
{
  spdlog::level::level_enum level = spdlog::level::info;
  char log_mode = 'c';
  bool specifiedLogMode = false;
  while (1)
  {
    int result = getopt(argc, argsv, "mhe:f:d:l:v:");
    if (result == -1)
    {
      break;
    }
    switch (result)
    {
    case 'v':
    {
      if (Input::isValidLogLevel(optarg))
      {
        char arg = optarg[0];
        switch (arg)
        {
        case 'o':
          level = spdlog::level::off;
          break;
        case 'e':
          level = spdlog::level::err;
          break;
        case 'c':
          level = spdlog::level::critical;
          break;
        case 'w':
          level = spdlog::level::warn;
          break;
        case 'i':
          level = spdlog::level::info;
          break;
        case 'd':
          level = spdlog::level::debug;
          break;
        case 't':
          level = spdlog::level::trace;
          break;
        }
      }
      else
      {
        std::cout << "Error: Please specify a valid log level" << std::endl;
        printHelp();
        exit(0);
      }
    }
    break;
    case 'l':
      if (Input::isValidLogMode(optarg))
      {
        log_mode = optarg[0];
        specifiedLogMode = true;
      }
      else
      {
        std::cout << "Error: Please specify a valid log mode" << std::endl;
        printHelp();
        exit(0);
      }
            //break; if this stays maybe loggers are not turned off in benchmark mode
        }
        if (result == 'b')
        {
            level = spdlog::level::off;
      break;
    case 'm':
      if (!specifiedLogMode)
      {
        log_mode = 'f';
      }
      break;
    }
  }
  optind = 1;
  Logger::initializeLoggers(level, log_mode);
}

const void handleInput(int argc, char *argsv[])
{
  while (1)
  {
    int result = getopt(argc, argsv, "mhe:f:d:l:v:b:");

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
    case 'e':
      if (Input::isDouble(optarg))
      {
        programParameters->setEndTime(std::__cxx11::stod(optarg));
      }
      else
      {
        std::cout << "Error: end_time parameter (-e) is not a double" << std::endl;
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
        case 'b':
        {
            if (Input::isInt(optarg)) {
                programParameters->setBenchmarkIterations(std::__cxx11::stoi(optarg));
                programParameters->setMode(Mode::Benchmark);
            }
            else {
                std::cout << "Error: benchmark run parameter (-b) is not an int" << std::endl;
                printHelp();
                exit(0);
            }
        }
        break;
    default:
      break;
    }
  }
}

void printHelp()
{
  printf(" -f <filename> .......... The path to an input file. If not specified and no cuboids are generated, no particles appear in the simulation.\n");
  printf(" -e <end_time> .......... The end time of the simulation. If not specified, 100 is used\n");
  printf(" -d <delta_t> ........... The size of the time steps in the simulation. If not specified 0.014 is used\n");
  printf(" -v <verbosity_level>.... Sets the verbosity level for the program: 'o' (off), 'e' (error), 'c' (critical), 'w' (warn), 'i' (info), 'd' (debug), 't'. By default info is used (trace)\n");
  printf(" -l <log_mode>........... Specifies where the logs for the program are written to: 'f' (file), 'c' (console). By default, logs are written to the console when opening the menu\n");
    printf(" -b <runs>............... Activate benchmark mode, compute mean simulation time over given number of runs. Overwrites any log-level specification to turn all loggers off\n");
  printf(" -m ..................... Enter the console menu, here you can read in files, create cuboids and re-run the program with the same parameters. If the menu is specified, logs are written to files by default\n");
  printf(" -h ..................... Help\n");
}
