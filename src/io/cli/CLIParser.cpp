#include "CLIParser.h"

#include <boost/program_options.hpp>

#include "io/logger/Logger.h"
#include "io/output/FileOutputHandler.h"
#include "spdlog/sinks/rotating_file_sink.h"

SimulationParams parse_arguments(int argc, char* argsv[]) {
    std::string input_file_path;
    std::string output_dir_path;
    std::string log_level;
    std::string output_format;
    std::string log_output;

    double end_time = 0;
    double delta_t = 0;

    int fps = 0;
    int video_length = 0;

    // choosing 0 as one of the parameters (end_time, delta_t, fps, video_length) is equivalent to choosing the default value
    boost::program_options::options_description options_desc("Allowed options");
    options_desc.add_options()("help,h", "produce help message");
    options_desc.add_options()(
        "input_file_path,f", boost::program_options::value<std::string>(&input_file_path),
        "The path to the input file. Must be specified, otherwise the program will terminate. Can be inserted as positional argument.");
    options_desc.add_options()("output_dir_path,o", boost::program_options::value<std::string>(&output_dir_path)->default_value(""),
                               "The path to the directory in which to save the simulation output files"
                               "Default: './output/<input_file_name>/'.\n"
                               "NOTE: The directory  will be cleared before execution!!!");
    options_desc.add_options()("delta_t,d", boost::program_options::value<double>(&delta_t), "The time step per simulation iteration");
    options_desc.add_options()("end_time,e", boost::program_options::value<double>(&end_time),
                               "The time, at which the simulation will end");
    options_desc.add_options()("fps", boost::program_options::value<int>(&fps),
                               "The number of frames per second at which the simulation will be saved");
    options_desc.add_options()("video_length", boost::program_options::value<int>(&video_length),
                               "The total length of the simulation video in seconds");
    options_desc.add_options()("log_level,l", boost::program_options::value<std::string>(&log_level)->default_value("info"),
                               "The log level. Possible values: trace, debug, info, warning, error, critical, off");
    options_desc.add_options()("output_format", boost::program_options::value<std::string>(&output_format)->default_value("vtk"),
                               "The output format. Possible values: vtk, xyz, none");
    options_desc.add_options()("log_output", boost::program_options::value<std::string>(&log_output)->default_value("std"),
                               "You can only choose between the output options std(only cl output) and file (only file output). Default: no file output");

    boost::program_options::positional_options_description positional_options_desc;
    positional_options_desc.add("input_file_path", -1);

    boost::program_options::variables_map variables_map;
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argsv).options(options_desc).positional(positional_options_desc).run(),
        variables_map);
    boost::program_options::notify(variables_map);

    if(log_output == "std" || log_output == "STD"){
        Logger::logger->info("Log output: std");
    } else if(log_output == "file" || log_output == "FILE"){
        Logger::logger = Logger::init_logger(LogType::FILE);
        Logger::logger->info("Log output: file");
    } else {
        std::cout << "Error: Invalid log output given. Options: no file output: 'std' and file output: 'file'" << std::endl;
        exit(-1);
    }

    if (log_level == "trace") {
        Logger::logger->set_level(spdlog::level::trace);
    } else if (log_level == "debug") {
        Logger::logger->set_level(spdlog::level::debug);
    } else if (log_level == "info") {
        Logger::logger->set_level(spdlog::level::info);
    } else if (log_level == "warning") {
        Logger::logger->set_level(spdlog::level::warn);
    } else if (log_level == "error") {
        Logger::logger->set_level(spdlog::level::err);
    } else if (log_level == "critical") {
        Logger::logger->set_level(spdlog::level::critical);
    } else if (log_level == "off") {
        Logger::logger->set_level(spdlog::level::off);
    } else {
        std::cout << "Error: Invalid log level given." << std::endl;
        exit(-1);
    }

    if (argc <= 1 || variables_map.count("help")) {
        std::stringstream help_message;
        help_message << options_desc << std::endl;
        Logger::logger->info(help_message.str());
        exit(-1);
    }
    if (!variables_map.count("input_file_path")) {
        Logger::logger->error("No input file path given.");
        std::stringstream help_message;
        help_message << options_desc << std::endl;
        Logger::logger->info(help_message.str());
        exit(-1);
    }

    return SimulationParams{input_file_path, output_dir_path, delta_t, end_time, fps, video_length, SimulationParams::DirectSumType{},
                            output_format};
}

SimulationParams merge_parameters(const SimulationParams& params_cli, const SimulationParams& params_xml) {
    SimulationParams params = params_xml;

    // Overwrite parameters from XML file with parameters from CLI
    if (params_cli.delta_t != 0) {
        params.delta_t = params_cli.delta_t;
    }
    if (params_cli.end_time != 0) {
        params.end_time = params_cli.end_time;
    }
    if (params_cli.fps != 0) {
        params.fps = params_cli.fps;
    }
    if (params_cli.video_length != 0) {
        params.video_length = params_cli.video_length;
    }

    // Always takes value from CLI
    params.output_dir_path = params_cli.output_dir_path;

    // Must be given in the CLI
    params.output_format = params_cli.output_format;

    return params;
}