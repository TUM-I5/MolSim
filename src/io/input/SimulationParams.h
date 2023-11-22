#pragma once

#include <string>

class SimulationParams {
   public:
    std::string input_file_path;
    std::string output_dir_path;

    double delta_t;
    double end_time;

    int fps;
    int video_length;

    std::string log_level;

    SimulationParams(const std::string& input_file_path, const std::string& output_dir_path, double delta_t, double end_time, int fps,
                     int video_length, const std::string& log_level)
        : input_file_path(input_file_path),
          output_dir_path(output_dir_path),
          delta_t(delta_t),
          end_time(end_time),
          fps(fps),
          video_length(video_length),
          log_level(log_level) {}

    SimulationParams() {}
};