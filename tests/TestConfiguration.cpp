//
// Created by alex on 06.12.22.
//

#include <gtest/gtest.h>

#include "io/input/arg_names.h"
#include "io/input/Configuration.h"

/**
 * Sets the cli args to test values
 * */
static void setCLIArgs() {
    using namespace io::input;
    using namespace sim::physics;
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-of")).value = "outputFolder";
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-o")).value = "outputName";
    std::get<ArgEntry<double>>(cli_arg_map.at("-st")).value = 2.0;
    std::get<ArgEntry<double>>(cli_arg_map.at("-et")).value = 10.1;
    std::get<ArgEntry<double>>(cli_arg_map.at("-dt")).value = 0.25;
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-f")).value = "lennardjonesomp";
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-x")).value = "stoermervelvetomp";
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-v")).value = "stoermervelvetomp";
    std::get<ArgEntry<double>>(cli_arg_map.at("-sig")).value = 1.5;
    std::get<ArgEntry<double>>(cli_arg_map.at("-eps")).value = 1.8;
    std::get<ArgEntry<double>>(cli_arg_map.at("-brown")).value = 1.3;
    std::get<ArgEntry<int>>(cli_arg_map.at("-lc")).value = 4;
    std::get<ArgEntry<double>>(cli_arg_map.at("-rc")).value = 5.0;
    std::get<ArgEntry<double>>(cli_arg_map.at("-bbox0")).value = 55.0;
    std::get<ArgEntry<double>>(cli_arg_map.at("-bbox1")).value = 53.0;
    std::get<ArgEntry<double>>(cli_arg_map.at("-bbox2")).value = 51.0;
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-bndFront")).value = "outflow";
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-bndRear")).value = "outflow";
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-bndLeft")).value = "reflecting";
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-bndRight")).value = "reflecting";
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-bndTop")).value = "reflecting";
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-bndBottom")).value = "outflow";
    std::get<ArgEntry<int>>(cli_arg_map.at("-dims")).value = 3;
    std::get<ArgEntry<int>>(cli_arg_map.at("-llv")).value = 0;
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-bench")).isSet = true;
    std::get<ArgEntry<std::string>>(cli_arg_map.at("-bench")).value = "file";
    std::get<ArgEntry<int>>(cli_arg_map.at("-bMax")).value = 102;
    std::get<ArgEntry<int>>(cli_arg_map.at("-i")).value = 16;
}

/**
 * Checks if all fields are set with data after calling load cli.
 * */
TEST(Configuration, loadCLIAllSet) {
    setCLIArgs();
    io::input::Configuration config;
    config.loadCLIArgs();

    //check if all fields are set
    auto& data = config.getData();
    auto& locks = config.getLocks();
    for(int i {0}; i < io::input::names::names_count; i++) {
        EXPECT_TRUE(data.contains(static_cast<io::input::names>(i)));
        EXPECT_TRUE(locks.contains(static_cast<io::input::names>(i)));
    }
}

TEST(Configuration, loadCLICorrect) {
    using namespace io::input;
    setCLIArgs();
    io::input::Configuration config;
    config.loadCLIArgs();
    config.get<outputFilePath>();
    config.get<outputFileName>();
    config.get<startTime>();
    config.get<endTime>();
    config.get<delta_t>();
    config.get<forceCalculation>();
    config.get<positionCalculation>();
    config.get<velocityCalculation>();
    config.get<sigma>();
    config.get<epsilon>();
    config.get<brown>();
    config.get<linkedCell>();
    config.get<rCutoff>();
    config.get<boundingBox_X0>();
    config.get<boundingBox_X1>();
    config.get<boundingBox_X2>();
    config.get<boundCondFront>();
    config.get<boundCondRear>();
    config.get<boundCondLeft>();
    config.get<boundCondRight>();
    config.get<boundCondTop>();
    config.get<boundCondBottom>();
    config.get<dimensions>();
    config.get<logLevel>();
    config.get<benchmark>();
    config.get<benchmarkType>();
    config.get<benchMaxBodySize>();
    config.get<benchIterationCount>();
}