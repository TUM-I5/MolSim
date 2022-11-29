//
// Created by alex on 26.11.2022.
//

#include <filesystem>
#include "defaults.h"
#include "CLIArgs.h"
#include "io/output/Logging.h"
#include "CLIArgsParser.h"

namespace io::input {

    cli_arg_map_t cli_arg_map = {
            {"-llv",
                    ArgEntry<int>(
                            "-llv",
                            "--loglevel",
                            "Set log level. 0: trace 1: debug 2: info 3: warn 4: err 5: critical 6: off",
                            "<int>",
                            true,
                            io::output::loggers::level::info,
                            [](std::string &arg) {
                                int lv = std::stoi(arg);
                                if (lv >= io::output::loggers::level::level_count)
                                    lv = io::output::loggers::level_count - 1;
                                else if (lv < 0) lv = 0;
                                return lv;
                            },
                            [](const int &lv) {
                                io::output::loggers::init(static_cast<io::output::loggers::level>(lv));
                            }
                    )},
            {"-dt",
                    ArgEntry<double>(
                            "-dt",
                            "--deltaTime",
                            "Sets delta time to <value>. If -dt is not specified default value is used.",
                            "<value>",
                            true,
                            default_delta_t,
                            [](std::string &arg) { return std::stod(arg); }
                    )},
            {"-et",
                    ArgEntry<double>(
                            "-et",
                            "--endTime",
                            "Set end time to <value>. If -et is not specified default value is used.",
                            "<value>",
                            true,
                            default_end_time,
                            [](std::string &arg) { return std::stod(arg); }
                    )},
            {"-st",
                    ArgEntry<double>(
                            "-st",
                            "--startTime",
                            "Sets start time to <value>. If -st is not specified default value is used.",
                            "<value>",
                            true,
                            default_start_time,
                            [](std::string &arg) { return std::stod(arg); }
                    )},
            {"-sig",
                    ArgEntry<double>(
                            "-sig",
                            "--sigma",
                            "Sets sigma to <value>. If -sig is not specified default value is used.",
                            "<value>",
                            true,
                            default_sigma,
                            [](std::string &arg) { return std::stod(arg); }
                    )},
            {"-eps",
                    ArgEntry<double>(
                            "-eps",
                            "--epsilon",
                            "Sets epsilon to <value>. If -eps is not specified default value is used.",
                            "<value>",
                            true,
                            default_epsilon,
                            [](std::string &arg) { return std::stod(arg); }
                    )},
            {"-o",
                    ArgEntry<std::string>(
                            "-o",
                            "--outputBaseName",
                            "Set base name of output files. DO NOT USE A PATH! Default is 'result'.",
                            "<name>",
                            true,
                            default_output_base_name,
                            [](std::string &arg) { return arg; }
                    )},
            {"-of",
                    ArgEntry<std::string>(
                            "-of",
                            "--outputFolderName",
                            "Set path to output folder. Default is ./output",
                            "<path>",
                            true,
                            default_output_folder,
                            [](std::string &arg) {
                                std::string of = arg;
                                if (!of.ends_with("/")) of = of.append("/");
                                return of;
                            },
                            [](const std::string &outputFolder) {
                                if (!std::filesystem::exists(outputFolder))
                                    std::filesystem::create_directory(outputFolder);
                                else if (!std::filesystem::is_directory(outputFolder))
                                    io::input::exitFormatError(outputFolder + ": is not a directory!");
                            }
                    )},
            {"-bench",
                    ArgEntry<std::string>(
                            "-bench",
                            "--benchmark",
                            "Enable benchmarking. <type> is either 'default' or 'file'. With default an internal hard coded benchmark is performed. With file user supplied input files will be used.",
                            "<type>",
                            true,
                            "default",
                            [](std::string &arg) { return arg; }
                            // we do NOT want to call runBenchmark at this point yet, to load the other args also
                    )},
            {"-i",
                    ArgEntry<int>(
                            "-i",
                            "--iterations",
                            "Set how many passes should be done for each benchmark",
                            "<value>",
                            true,
                            default_bench_iterations,
                            [](std::string &arg) { return std::stoi(arg); }
                            // we do NOT want to call runBenchmark at this point yet, to load the other args also
                    )},
            {"-bMax",
                    ArgEntry<int>(
                            "-bMax",
                            "--bodyMaxSize",
                            "Set maximum body size for default benchmark",
                            "<value>",
                            true,
                            default_bench_maxBody,
                            [](std::string &arg) { return std::stoi(arg); }
                            // we do NOT want to call runBenchmark at this point yet, to load the other args also
                    )},
            {"-bnd",
                    ArgEntry<std::string>(
                            "-bnd",
                            "--boundaryCond",
                            "Defines the boundary condition behaviour in linked cell mode. 'outflow' : removes particles upon crossing domain bounds 'reflecting' : reflects particles off of domain bounds, default is outflow.",
                            "<type>",
                            true,
                            default_boundary_cond_str,
                            [](std::string &arg) { return arg; }
                    )},
            {"-lc",
                    ArgEntry<int>(
                            "-lc",
                            "--linkedCell",
                            "Enables linked cell operation. Linked Cell is on by default. Set <bool> to 0 for false and to 1 for true",
                            "<bool>",
                            true,
                            default_linked_cell,
                            [](std::string &arg) { return std::stoi(arg); }
                    )}
    };

} // io::input
