#include "gtest/gtest.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

class MyEnvironment : public ::testing::Environment
{
public:
  void SetUp() override
  {

    try
    {
      spdlog::level::level_enum level = spdlog::level::debug;
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

      std::string input_filename = "../logs/" + prefix.str() + "input.txt";
      auto input_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(input_filename, false);
      auto input_logger = std::make_shared<spdlog::logger>("input_logger", input_sink);
      input_logger->set_level(level);
      spdlog::register_logger(input_logger);

      std::string output_filename = "../logs/" + prefix.str() + "output.txt";
      auto output_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(output_filename, false);
      auto output_logger = std::make_shared<spdlog::logger>("output_logger", output_sink);
      output_logger->set_level(level);
      spdlog::register_logger(output_logger);

      std::string memory_filename = "../logs/" + prefix.str() + "memory.txt";
      auto memory_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(memory_filename, false);
      auto memory_logger = std::make_shared<spdlog::logger>("memory_logger", memory_sink);
      memory_logger->set_level(level);
      spdlog::register_logger(memory_logger);
    }
    catch (const spdlog::spdlog_ex &ex)
    {
      std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
  }
};

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new MyEnvironment);
  return RUN_ALL_TESTS();
}