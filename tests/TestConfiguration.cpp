//
// Created by alex on 06.12.22.
//

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "io/input/arg_names.h"
#include "io/input/Configuration.h"
#include "io/IOWrapper.h"


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
    std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-thermo")).value = 1;
    std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-ti")).value = 2.0;
    std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-nt")).value = 200;
    std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-tt")).value = 2.5;
    std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-dTemp")).value = 2.6;
    std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-cp")).value = 1;
    std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-gGrav")).value = 5.0;
    std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-lastIt")).value = 1000;
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

/**
 * Checks if all given arguments are loaded correctly
 * */
TEST(Configuration, loadCLICorrect) {
    using namespace io::input;
    setCLIArgs();
    io::input::Configuration config;
    config.loadCLIArgs();
    EXPECT_EQ(config.get<outputFilePath>(), "outputFolder");
    EXPECT_EQ(config.get<outputFileName>(), "outputName");
    EXPECT_EQ(config.get<startTime>(), 2.0);
    EXPECT_EQ(config.get<endTime>(), 10.1);
    EXPECT_EQ(config.get<delta_t>(), 0.25);
    EXPECT_EQ(config.get<forceCalculation>(), sim::physics::force::type::lennardJonesOMP);
    EXPECT_EQ(config.get<positionCalculation>(), sim::physics::position::type::stoermerVelvetOMP);
    EXPECT_EQ(config.get<velocityCalculation>(), sim::physics::position::type::stoermerVelvetOMP);
    EXPECT_EQ(config.get<sigma>(), 1.5);
    EXPECT_EQ(config.get<epsilon>(), 1.8);
    EXPECT_EQ(config.get<brown>(), 1.3);
    EXPECT_EQ(config.get<linkedCell>(), true);
    EXPECT_EQ(config.get<rCutoff>(), 5.0);
    EXPECT_EQ(config.get<boundingBox_X0>(), 55.0);
    EXPECT_EQ(config.get<boundingBox_X1>(), 53.0);
    EXPECT_EQ(config.get<boundingBox_X2>(), 51.0);
    EXPECT_EQ(config.get<boundCondFront>(), sim::physics::bounds::type::outflow);
    EXPECT_EQ(config.get<boundCondRear>(), sim::physics::bounds::type::outflow);
    EXPECT_EQ(config.get<boundCondLeft>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<boundCondRight>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<boundCondTop>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<boundCondBottom>(), sim::physics::bounds::type::outflow);
    EXPECT_EQ(config.get<dimensions>(), 3);
    EXPECT_EQ(config.get<logLevel>(), 0);
    EXPECT_EQ(config.get<benchmark>(), true);
    EXPECT_EQ(config.get<benchmarkType>(), "file");
    EXPECT_EQ(config.get<benchMaxBodySize>(), 102);
    EXPECT_EQ(config.get<benchIterationCount>(), 16);
    EXPECT_EQ(config.get<thermoEnable>(), true);
    EXPECT_EQ(config.get<thermoTInit>(), 2.0);
    EXPECT_EQ(config.get<thermoNTerm>(), 200);
    EXPECT_EQ(config.get<thermoTTarget>(), 2.5);
    EXPECT_EQ(config.get<thermoDelta_t>(), 2.6);
    EXPECT_EQ(config.get<checkpointingEnable>(), true);
    EXPECT_EQ(config.get<gGrav>(), 5.0);
    EXPECT_EQ(config.get<simLastIteration>(), 1000);
}

/**
 * Sets the argMap with predefined values.
 * */
static void setFileMap(std::unordered_map<io::input::names, std::string>& argMap) {
    using namespace io::input;
    argMap[outputFilePath] = "filePath";
    argMap[outputFileName] = "fileName";
    argMap[startTime] = "0.0";
    argMap[endTime] = "1.0";
    argMap[delta_t] = "5.1";
    argMap[forceCalculation] = "lennardjones";
    argMap[positionCalculation] = "stoermervelvet";
    argMap[velocityCalculation] = "stoermervelvet";
    argMap[sigma] = "60.0";
    argMap[epsilon] = "92.0";
    argMap[brown] = "1.2";
    argMap[linkedCell] = "0";
    argMap[rCutoff] = "3.2";
    argMap[boundingBox_X0] = "22.0";
    argMap[boundingBox_X1] = "53.2";
    argMap[boundingBox_X2] = "34.0";
    argMap[boundCondFront] = "outflow";
    argMap[boundCondRear] = "outflow";
    argMap[boundCondLeft] = "outflow";
    argMap[boundCondRight] = "reflecting";
    argMap[boundCondTop] = "reflecting";
    argMap[boundCondBottom] = "reflecting";
    argMap[dimensions] = "2";
    argMap[logLevel] = "3";
    argMap[benchmark] = "0";
    argMap[benchmarkType] = "default";
    argMap[benchMaxBodySize] = "30";
    argMap[benchIterationCount] = "42";
    argMap[thermoEnable] = "0";
    argMap[thermoTInit] = "5.0";
    argMap[thermoNTerm] = "233";
    argMap[thermoTTarget] = "3.0";
    argMap[thermoDelta_t] = "1.0";
    argMap[checkpointingEnable] = "0";
    argMap[gGrav] = "0.1";
    argMap[simLastIteration] = "1";
}

/**
 * Checks if args from file are loaded and are correct
 * */
TEST(Configuration, loadFileSet) {
    using namespace io::input;
    setCLIArgs();
    io::input::Configuration config;
    config.loadCLIArgs();
    auto& data = config.getData();
    auto& locks = config.getLocks();
    //unlocks locks to set file args
    for (auto& [key, b] : locks) b = false;

    std::unordered_map<names, std::string> argMap;
    setFileMap(argMap);
    config.loadIOWArgs(argMap);

    //perform check
    EXPECT_EQ(config.get<outputFilePath>(), "filePath");
    EXPECT_EQ(config.get<outputFileName>(), "fileName");
    EXPECT_EQ(config.get<startTime>(), 0.0);
    EXPECT_EQ(config.get<endTime>(), 1.0);
    EXPECT_EQ(config.get<delta_t>(), 5.1);
    EXPECT_EQ(config.get<forceCalculation>(), sim::physics::force::type::lennardJones);
    EXPECT_EQ(config.get<positionCalculation>(), sim::physics::position::type::stoermerVelvet);
    EXPECT_EQ(config.get<velocityCalculation>(), sim::physics::position::type::stoermerVelvet);
    EXPECT_EQ(config.get<sigma>(), 60.0);
    EXPECT_EQ(config.get<epsilon>(), 92.0);
    EXPECT_EQ(config.get<brown>(), 1.2);
    EXPECT_EQ(config.get<linkedCell>(), false);
    EXPECT_EQ(config.get<rCutoff>(), 3.2);
    EXPECT_EQ(config.get<boundingBox_X0>(), 22.0);
    EXPECT_EQ(config.get<boundingBox_X1>(), 53.2);
    EXPECT_EQ(config.get<boundingBox_X2>(), 34.0);
    EXPECT_EQ(config.get<boundCondFront>(), sim::physics::bounds::type::outflow);
    EXPECT_EQ(config.get<boundCondRear>(), sim::physics::bounds::type::outflow);
    EXPECT_EQ(config.get<boundCondLeft>(), sim::physics::bounds::type::outflow);
    EXPECT_EQ(config.get<boundCondRight>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<boundCondTop>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<boundCondBottom>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<dimensions>(), 2);
    EXPECT_EQ(config.get<logLevel>(), 3);
    EXPECT_EQ(config.get<benchmark>(), false);
    EXPECT_EQ(config.get<benchmarkType>(), "default");
    EXPECT_EQ(config.get<benchMaxBodySize>(), 30);
    EXPECT_EQ(config.get<benchIterationCount>(), 42);
    EXPECT_EQ(config.get<thermoEnable>(), false);
    EXPECT_EQ(config.get<thermoTInit>(), 5.0);
    EXPECT_EQ(config.get<thermoNTerm>(), 233);
    EXPECT_EQ(config.get<thermoTTarget>(), 3.0);
    EXPECT_EQ(config.get<thermoDelta_t>(), 1.0);
    EXPECT_EQ(config.get<checkpointingEnable>(), false);
    EXPECT_EQ(config.get<gGrav>(), 0.1);
    EXPECT_EQ(config.get<simLastIteration>(), 1);
}

/**
 * Check if the config class integrates with the BodyReader.
 * We only expect it to load values for epsilon, sigma, brown and dims
 * */
TEST(Configuration, integrationBodyReader) {
    //init config
    using namespace io::input;
    setCLIArgs();
    io::input::Configuration config;
    config.loadCLIArgs();
    auto& data = config.getData();
    auto& locks = config.getLocks();
    //unlocks locks to set file args
    for (auto& [key, b] : locks) b = false;

    //handle file
    std::string uri{"BodyReaderInput.txt"};
    auto ioWrapper = io::IOWrapper(BODY, uri);
    ioWrapper.reload();
    config.loadIOWArgs(ioWrapper.getArgMap());

    //handle checks
    EXPECT_EQ(config.get<epsilon>(), 5.0);
    EXPECT_EQ(config.get<sigma>(), 1.0);
    EXPECT_EQ(config.get<brown>(), 0.1);
    EXPECT_EQ(config.get<dimensions>(), 2);
}

/**
 * Checks if Configuration loads input from XMLReader correctly.
 * */
TEST(Configuration, integrationXMLReader) {
    //init config
    using namespace io::input;
    setCLIArgs();
    io::input::Configuration config;
    config.loadCLIArgs();
    auto& data = config.getData();
    auto& locks = config.getLocks();
    //unlocks locks to set file args
    for (auto& [key, b] : locks) b = false;

    //handle file
    std::string uri{"XMLReaderInput.xml"};
    auto ioWrapper = io::IOWrapper(XML, uri);
    ioWrapper.reload();
    config.loadIOWArgs(ioWrapper.getArgMap());

    //handle checks
    EXPECT_EQ(config.get<outputFilePath>(), "tmpPath");
    EXPECT_EQ(config.get<outputFileName>(), "tmpOutput");
    EXPECT_EQ(config.get<startTime>(), 5.0);
    EXPECT_EQ(config.get<endTime>(), 10.0);
    EXPECT_EQ(config.get<delta_t>(), 2.0);
    EXPECT_EQ(config.get<forceCalculation>(), sim::physics::force::type::lennardJonesCell);
    EXPECT_EQ(config.get<epsilon>(), 5.0);
    EXPECT_EQ(config.get<sigma>(), 1.0);
    EXPECT_EQ(config.get<positionCalculation>(), sim::physics::position::type::stoermerVelvetOMP);
    EXPECT_EQ(config.get<velocityCalculation>(), sim::physics::velocity::type::stoermerVelvetOMP);
    EXPECT_EQ(config.get<brown>(), 5.0);
    EXPECT_EQ(config.get<linkedCell>(), true);
    EXPECT_EQ(config.get<boundingBox_X0>(), 120.0);
    EXPECT_EQ(config.get<boundingBox_X1>(), 50.0);
    EXPECT_EQ(config.get<boundingBox_X2>(), 10.0);
    EXPECT_EQ(config.get<boundCondFront>(), sim::physics::bounds::type::outflow);
    EXPECT_EQ(config.get<boundCondRear>(), sim::physics::bounds::type::outflow);
    EXPECT_EQ(config.get<boundCondLeft>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<boundCondRight>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<boundCondTop>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<boundCondBottom>(), sim::physics::bounds::type::reflecting);
    EXPECT_EQ(config.get<rCutoff>(), 3.0);
    EXPECT_EQ(config.get<dimensions>(), 3);
    EXPECT_EQ(config.get<logLevel>(), 5);
    EXPECT_EQ(config.get<benchmark>(), true);
    EXPECT_EQ(config.get<benchmarkType>(), "default");
    EXPECT_EQ(config.get<benchMaxBodySize>(), 100);
    EXPECT_EQ(config.get<benchIterationCount>(), 10000);
    EXPECT_EQ(config.get<thermoEnable>(), true);
    EXPECT_EQ(config.get<thermoTInit>(), 50.0);
    EXPECT_EQ(config.get<thermoNTerm>(), 987);
    EXPECT_EQ(config.get<thermoTTarget>(), 30.0);
    EXPECT_EQ(config.get<thermoDelta_t>(), 576.3);
    EXPECT_EQ(config.get<checkpointingEnable>(), true);
    EXPECT_EQ(config.get<gGrav>(), 5.0);
    EXPECT_EQ(config.get<simLastIteration>(), 1000);
}