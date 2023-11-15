#include "CLIParser.h"

#include <boost/program_options.hpp>

#include "io/logger/Logger.h"

std::string construct_output_path(const std::string& input_file_path) {
    auto last_slash_pos = input_file_path.find_last_of('/');
    auto last_dot_pos = input_file_path.find_last_of('.');
    if (last_slash_pos == std::string::npos) {
        last_slash_pos = 0;
    }
    if (last_dot_pos == std::string::npos) {
        last_dot_pos = input_file_path.size();
    }
    return "./output/" + input_file_path.substr(last_slash_pos + 1, last_dot_pos - last_slash_pos - 1) + "/";
};

std::tuple<std::string, std::string, double, double, int, int, std::string> parse_arguments(int argc, char* argsv[]) {
    std::string input_file_path;
    std::string output_dir_path;

    double end_time;
    double delta_t;

    int fps;
    int video_length;
    std::string log_level;

    boost::program_options::options_description options_desc("Allowed options");
    options_desc.add_options()("help,h", "produce help message");
    options_desc.add_options()("input_file_path,f", boost::program_options::value<std::string>(&input_file_path),
                               "The path to the input file. Must be specified, otherwise the program will terminate. Can be inserted as positional argument.");
    options_desc.add_options()("output_dir_path,o", boost::program_options::value<std::string>(&output_dir_path),
                               "The path to the directory in which to save the simulation output files"
                               "Default: './output/<input_file_name>/'.\n"
                               "NOTE: The directory  will be cleared before execution!!!");
    options_desc.add_options()("delta_t,d", boost::program_options::value<double>(&delta_t)->default_value(0.014), "The time step per simulation iteration");
    options_desc.add_options()("end_time,e", boost::program_options::value<double>(&end_time)->default_value(1000),
                               "The time, at which the simulation will end");
    options_desc.add_options()("fps", boost::program_options::value<int>(&fps)->default_value(24),
                               "The number of frames per second at which the simulation will be saved");
    options_desc.add_options()("video_length", boost::program_options::value<int>(&video_length)->default_value(30),
                               "The total length of the simulation video in seconds");
    options_desc.add_options()("log_level,l", boost::program_options::value<std::string>(&log_level)->default_value("info"),
                               "The log level. Possible values: trace, debug, info, warning, error, critical, off");

    boost::program_options::positional_options_description positional_options_desc;
    positional_options_desc.add("input_file_path", -1);

    boost::program_options::variables_map variables_map;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argsv).options(options_desc).positional(positional_options_desc).run(),
                                  variables_map);
    boost::program_options::notify(variables_map);

    if(log_level == "trace") {
        Logger::logger->set_level(spdlog::level::trace);
    } else if(log_level == "debug") {
        Logger::logger->set_level(spdlog::level::debug);
    } else if(log_level == "info") {
        Logger::logger->set_level(spdlog::level::info);
    } else if(log_level == "warning") {
        Logger::logger->set_level(spdlog::level::warn);
    } else if(log_level == "error") {
        Logger::logger->set_level(spdlog::level::err);
    } else if(log_level == "critical") {
        Logger::logger->set_level(spdlog::level::critical);
    } else if(log_level == "off") {
        Logger::logger->set_level(spdlog::level::off);
    } else {
        std::cout << "Invalid log level given." << std::endl;
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
    if (delta_t <= 0) {
        Logger::logger->error("Timestep delta_t must be greater than 0.");
        exit(-1);
    }
    if (end_time <= 0) {
        Logger::logger->error("End time must be greater than 0.");
        exit(-1);
    }
    if (fps <= 0) {
        Logger::logger->error("fps must be greater than 0.");
        exit(-1);
    }
    if (video_length <= 0) {
        Logger::logger->error("video length must be greater than 0.");
        exit(-1);
    }

    if (!variables_map.count("output_dir_path")) {
        output_dir_path = construct_output_path(input_file_path);
    }

    return std::make_tuple(input_file_path, output_dir_path, delta_t, end_time, fps, video_length, log_level);
}
