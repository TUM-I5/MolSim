#include <filesystem>
#include <iostream>
#include "Logger.h"

#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

std::shared_ptr<spdlog::logger> Logger::logger = init_logger();

void createDirectory(const std::string& dir_name) {
    if (!std::filesystem::exists(dir_name)) {
        try {
            std::filesystem::create_directory(dir_name);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error creating directory: " << e.what() << std::endl;
            exit(-1);
        }
    }
}

std::shared_ptr<spdlog::logger> Logger::init_logger(LogType log_type) {
    auto standard_out = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    standard_out->set_color(spdlog::level::critical, standard_out->dark);
    standard_out->set_color(spdlog::level::err, standard_out->red);
    standard_out->set_color(spdlog::level::warn, standard_out->yellow);
    standard_out->set_color(spdlog::level::info, standard_out->green);
    standard_out->set_color(spdlog::level::debug, standard_out->blue);
    standard_out->set_color(spdlog::level::trace, standard_out->magenta);

    spdlog::init_thread_pool(8192, 1);
    std::shared_ptr<spdlog::logger> logger;
    if (log_type == LogType::FILE){
        createDirectory("logs");
        logger = spdlog::rotating_logger_st("file_logger","logs/log", 1048576 * 5, 3);
    }
    else{
        logger = std::make_shared<spdlog::async_logger>("std_logger", standard_out, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    }

    logger->set_level(spdlog::level::off);
    logger->set_pattern("[%H:%M:%S] %^[%l]%$ %v");

    return logger;
}