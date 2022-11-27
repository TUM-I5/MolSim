/*
 *  Logger.h
 *
 *  Created on: 27.11.2022
 *      Author: wohlrapp
 */

#pragma once

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/logger.h"

#include <ctime>
#include <list>
#include <iostream>

/**
 * @brief namespace for the logger, includes functions for flushing and initialization of spdlog loggers
 */
namespace Logger
{

  inline std::list<std::shared_ptr<spdlog::sinks::basic_file_sink_mt>> sinks; /// list that holds all sinks that are used in the program

  /**
   * @brief Initialises the logger, must be called before objects are created because otherwise segfaults occur
   * @param level gives the level which is set for all loggers
   */
  inline const void initializeLoggers(spdlog::level::level_enum level, char log_mode)
  {
    try
    {
      if (log_mode == 'c' || level == spdlog::level::off)
      {
        auto simulation_logger = spdlog::stdout_color_mt("simulation_logger");
        simulation_logger->set_level(level);

        auto input_logger = spdlog::stdout_color_mt("input_logger");
        input_logger->set_level(level);

        auto output_logger = spdlog::stdout_color_mt("output_logger");
        output_logger->set_level(level);

        auto memory_logger = spdlog::stdout_color_mt("memory_logger");
        memory_logger->set_level(level);
      }
      else
      {
        std::time_t time = std::time(0);
        std::tm *now = std::localtime(&time);
        std::ostringstream prefix;
        prefix << (now->tm_year + 1900) << '-'
               << (now->tm_mon + 1) << '-'
               << now->tm_mday << '-'
               << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << '_';

        std::string simulation_filename = "../logs/" + prefix.str() + "simulation.txt";
        auto simulation_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(simulation_filename, false);
        auto simulation_logger = std::make_shared<spdlog::logger>("simulation_logger", simulation_sink);
        simulation_logger->set_level(level);
        spdlog::register_logger(simulation_logger);
        sinks.emplace_back(simulation_sink);

        std::string input_filename = "../logs/" + prefix.str() + "input.txt";
        auto input_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(input_filename, false);
        auto input_logger = std::make_shared<spdlog::logger>("input_logger", input_sink);
        input_logger->set_level(level);
        spdlog::register_logger(input_logger);
        sinks.emplace_back(input_sink);

        std::string output_filename = "../logs/" + prefix.str() + "output.txt";
        auto output_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(output_filename, false);
        auto output_logger = std::make_shared<spdlog::logger>("output_logger", output_sink);
        output_logger->set_level(level);
        spdlog::register_logger(output_logger);
        sinks.emplace_back(output_sink);

        std::string memory_filename = "../logs/" + prefix.str() + "memory.txt";
        auto memory_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(memory_filename, false);
        auto memory_logger = std::make_shared<spdlog::logger>("memory_logger", memory_sink);
        memory_logger->set_level(level);
        spdlog::register_logger(memory_logger);
        sinks.emplace_back(memory_sink);
      }
    }
    catch (const spdlog::spdlog_ex &ex)
    {
      std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
  }

  /**
   * @brief flushes all sinks, so output is written before terminating the program
   */
  inline const void flushAll()
  {
    for (auto &sink : sinks)
    {
      sink->flush();
    }
  }

}