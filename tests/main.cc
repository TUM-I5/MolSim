#include "gtest/gtest.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include "../src/utils/Logger.h"


class MyEnvironment : public ::testing::Environment {
    public:
        void SetUp() override {
           Logger::initializeLoggers(spdlog::level::debug, 'f');
        }

};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new MyEnvironment);
  return RUN_ALL_TESTS();
}