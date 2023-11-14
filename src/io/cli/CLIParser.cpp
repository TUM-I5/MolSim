#include "CLIParser.h"

#include <boost/program_options.hpp>

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

std::tuple<std::string, std::string, double, double, size_t, size_t> parse_arguments(int argc, char* argsv[]) {
    std::string input_file_path;
    std::string output_dir_path;

    double end_time;
    double delta_t;

    size_t fps;
    size_t video_length;

    boost::program_options::options_description options_desc("Allowed options");
    options_desc.add_options()("help,h", "produce help message");
    options_desc.add_options()("input_file_path,f", boost::program_options::value<std::string>(&input_file_path),
                               "The path to the input file. Must be specified, otherwise the program will terminate. Can be inserted as positional argument.");
    options_desc.add_options()("output_dir_path,o", boost::program_options::value<std::string>(&output_dir_path),
                               "The path to the directory in which to save the simulation output files. If not specified, the output will be saved in './output/<input_file_name>/'.\n"
                               "NOTE: If the directory at the specified path does not exist, it will be created. If it exists, it will be cleared before execution!!!");
    options_desc.add_options()("delta_t,d", boost::program_options::value<double>(&delta_t)->default_value(0.014),
                               "The time step per simulation iteration");
    options_desc.add_options()("end_time,e", boost::program_options::value<double>(&end_time)->default_value(1000),
                               "The time, at which the simulation will end");
    options_desc.add_options()("fps", boost::program_options::value<size_t>(&fps)->default_value(24),
                               "The number of frames per second at which the simulation will be saved");
    options_desc.add_options()("video_length", boost::program_options::value<size_t>(&video_length)->default_value(30),
                               "The total length of the simulation video in seconds");

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
    if (fps <= 0) {
        std::cout << "Error: fps must be greater than 0." << std::endl;
        exit(-1);
    }
    if (video_length <= 0) {
        std::cout << "Error: video length must be greater than 0." << std::endl;
        exit(-1);
    }

    if (!variables_map.count("output_dir_path")) {
        output_dir_path = construct_output_path(input_file_path);
    }

    return std::make_tuple(input_file_path, output_dir_path, delta_t, end_time, fps, video_length);
}
