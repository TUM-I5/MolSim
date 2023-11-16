#include "Logger.h"

#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::logger = init_logger();

std::shared_ptr<spdlog::logger> Logger::init_logger() {
    auto standard_out = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    standard_out->set_color(spdlog::level::critical, standard_out->dark);
    standard_out->set_color(spdlog::level::err, standard_out->red);
    standard_out->set_color(spdlog::level::warn, standard_out->yellow);
    standard_out->set_color(spdlog::level::info, standard_out->green);
    standard_out->set_color(spdlog::level::debug, standard_out->blue);
    standard_out->set_color(spdlog::level::trace, standard_out->magenta);

    spdlog::init_thread_pool(8192, 1);
    std::shared_ptr<spdlog::logger> logger =
        std::make_shared<spdlog::async_logger>("logger", standard_out, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(logger);

    logger->set_level(spdlog::level::debug);
    logger->set_pattern("[%H:%M:%S] %^[%l]%$ %v");

    return logger;
}