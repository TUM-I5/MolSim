#include "SimulationParams.h"

#include "io/logger/Logger.h"

SimulationParams::SimulationParams(const std::string& input_file_path, const std::string& output_dir_path, double delta_t, double end_time,
                                   int fps, int video_length, const std::string& log_level, int container_type, double domain_size,
                                   double cutoff_radius)
    : input_file_path(input_file_path),
      output_dir_path(output_dir_path),
      delta_t(delta_t),
      end_time(end_time),
      fps(fps),
      video_length(video_length),
      log_level(log_level),
      container_type(container_type),
      domain_size(domain_size),
      cutoff_radius(cutoff_radius) {
    if (fps < 0) {
        Logger::logger->error("FPS must be positive");
        exit(-1);
    }
    if (video_length < 0) {
        Logger::logger->error("Video length must be positive");
        exit(-1);
    }
    if (end_time < 0) {
        Logger::logger->error("End time must be positive");
        exit(-1);
    }
    if (delta_t < 0) {
        Logger::logger->error("Delta t must be positive");
        exit(-1);
    }
    if (container_type < 0 || container_type > 2) {
        Logger::logger->error("Container type must be 1 or 2");
        exit(-1);
    }
    if (domain_size < 0) {
        Logger::logger->error("Domain size must be positive");
        exit(-1);
    }
    if (cutoff_radius < 0) {
        Logger::logger->error("Cutoff radius must be positive");
        exit(-1);
    }
}