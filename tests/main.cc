#include "gtest/gtest.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include "../src/utils/Logger.h"


class MyEnvironment : public ::testing::Environment {
    public:
        void SetUp() override {
          try 
          {
            auto simulation_logger = spdlog::basic_logger_mt("simulation_logger", "../logs/unittest_simulation.txt", true);
            auto input_logger = spdlog::basic_logger_mt("input_logger", "../logs/unittest_input.txt", true);
            auto output_logger = spdlog::basic_logger_mt("output_logger", "../logs/unittest_output.txt", true);
            auto memory_logger = spdlog::basic_logger_mt("memory_logger", "../logs/unittest_memory.text", true); 
          }
          catch (const spdlog::spdlog_ex& ex)
          {
              std::cout << "Log initialization failed: " << ex.what() << std::endl;
          }
        }

};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new MyEnvironment);
  return RUN_ALL_TESTS();
}