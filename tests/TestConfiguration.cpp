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
}

/**
 * Generates test input for the BodyReader in file: tmpBodyReaderInput.txt
 * # eps   sig     brown   dims
 *   5.0   1.0     0.1     2
 * */
static void writeBodyReaderInput() {
    std::ofstream file;
    file.open ("tmpBodyReaderInput.txt");
    file << "1\n";
    file << "0.0 0.0 0.0 " << "0.0 0.0 0.0 " << "1.0 " << "Cuboid " << "40 8 1 " << "1.0\n";
    file << "5.0 1.0 0.1 2\n";
    file.flush();
    file.close();
}

/**
 * Deletes temp file from BodyReader input
 * */
static void deleteBodyReaderInput() {
    if (std::filesystem::exists("tmpBodyReaderInput.txt")) std::filesystem::remove_all("tmpBodyReaderInput.txt");
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
    writeBodyReaderInput();
    auto ioWrapper = io::IOWrapper<io::input::BodyReader>("tmpBodyReaderInput.txt");
    ioWrapper.reload();
    deleteBodyReaderInput();
    config.loadIOWArgs(ioWrapper.getArgMap());

    //handle checks
    EXPECT_EQ(config.get<epsilon>(), 5.0);
    EXPECT_EQ(config.get<sigma>(), 1.0);
    EXPECT_EQ(config.get<brown>(), 0.1);
    EXPECT_EQ(config.get<dimensions>(), 2);
}

/**
 * Generates test input for the XMLReader in file: tmpXMLReaderInput.txt
 * Parameters are:
 * o = tmpPath of = tmpOutput
 * st = 5.0 et = 10.0 dt = 2.0
 * force = lennardjonescell
 * eps = 5.0 sig = 1.0
 * pos = svomp
 * vel = svomp
 * brown = 5.0
 * lc 1
 * bbox 120.0 50.0 10.0
 * bounds o o r r r r
 * rcutoff = 3.0
 * dims = 3
 * llv = 5
 * bench = 1
 * benchtype = default
 * bmax = 100
 * i = 10000
 * */
static void writeXMLReaderInput() {
    std::ofstream file;
    file.open("tmpXMLReaderInput.txt");

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<Simulation xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
    file << "            xsi:noNamespaceSchemaLocation=\"../../input/XMLFormat.xsd\">\n";
    file << "    <OutputFile FolderPath=\"tmpPath\" OutputFileName=\"tmpOutput\"/>\n";
    file << "    <StartTime>5.0</StartTime>\n";
    file << "    <EndTime>10.0</EndTime>\n";
    file << "    <TimeStepSize>2.0</TimeStepSize>\n";
    file << "    <ForceCalculation>\n";
    file << "        <LennardJonesCell Epsilon=\"5.0\" Sigma=\"1.0\"/>\n";
    file << "    </ForceCalculation>\n";
    file << "    <PositionCalculation>StoermerVelvetOMP</PositionCalculation>\n";
    file << "    <VelocityCalculation>StoermerVelvetOMP</VelocityCalculation>\n";
    file << "    <AverageBrownianMotion>5.0</AverageBrownianMotion>\n";
    file << "    <SimulationStrategy>\n";
    file << "        <LinkedCell>\n";
    file << "            <BoundaryBox>\n";
    file << "                <BoxSize X=\"120.0\" Y=\"50.0\" Z=\"10.0\"/>\n";
    file << "                <Front>Outflow</Front>\n";
    file << "                <Rear>Outflow</Rear>\n";
    file << "                <Left>Reflecting</Left>\n";
    file << "                <Right>Reflecting</Right>\n";
    file << "                <Top>Reflecting</Top>\n";
    file << "                <Bottom>Reflecting</Bottom>\n";
    file << "            </BoundaryBox>\n";
    file << "            <CutoffRadius>3.0</CutoffRadius>\n";
    file << "        </LinkedCell>\n";
    file << "    </SimulationStrategy>\n";
    file << "    <Dimensions>3</Dimensions>\n";
    file << "    <LogLevel>5</LogLevel>\n";
    file << "    <Benchmark>\n";
    file << "        <BenchmarkType>\n";
    file << "            <DefaultBenchmark MaximumBodySize=\"100\"/>\n";
    file << "        </BenchmarkType>\n";
    file << "        <IterationCount>10000</IterationCount>\n";
    file << "    </Benchmark>\n";
    file << "    <ShapeList>\n";
    file << "        <Shape>\n";
    file << "            <Sphere Radius=\"15\" Spacing=\"1.1225\" Mass=\"1.0\">\n";
    file << "                <Position X=\"60.0\" Y=\"25.0\" Z=\"0.0\"/>\n";
    file << "                <Velocity X=\"0.0\" Y=\"-10.0\" Z=\"0.0\"/>\n";
    file << "            </Sphere>\n";
    file << "        </Shape>\n";
    file << "    </ShapeList>\n";
    file << "</Simulation>\n";

    file.flush();
    file.close();

//    std::ofstream format;
//    format.open("XMLFormat.xsd");
//
//    format.flush();
//    format.close();
}

static void deleteXMLReaderInput() {
    if (std::filesystem::exists("tmpXMLReaderInput.txt")) std::filesystem::remove_all("tmpXMLReaderInput.txt");
}

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
    writeXMLReaderInput();
    auto ioWrapper = io::IOWrapper<io::input::XMLReader>("tmpXMLReaderInput.txt");
    ioWrapper.reload();
    deleteXMLReaderInput();
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
}

TEST(Configuration, testXMLReaderSigmaEpsilon) {
    using namespace io::input;
    std::list<Particle> particles;
    std::unordered_map<io::input::names, std::string> argMap;
    io::input::XMLReader::readFile("../../tests/XML_test_files/lennardJonesSigEps.xml", particles, argMap);
    
    EXPECT_EQ(argMap[epsilon], "42.000000") << "epsilon was set to " << argMap[epsilon] << " and not 42" << std::endl;
    EXPECT_EQ(argMap[sigma], "24.000000") << "sigma was set to " << argMap[sigma] << " and not 24" << std::endl;;
}


TEST(Configuration, testXMLPosCalcSetting) {
    using namespace io::input;
    std::list<Particle> particles;
    std::unordered_map<io::input::names, std::string> argMap;
    io::input::XMLReader::readFile("../../tests/XML_test_files/setPosCalc.xml", particles, argMap);
    
    EXPECT_EQ(argMap[positionCalculation], "StoermerVelvet");
}

TEST(Configuration, testXMLLogLevel) {
    using namespace io::input;
    std::list<Particle> particles;
    std::unordered_map<io::input::names, std::string> argMap;
    io::input::XMLReader::readFile("../../tests/XML_test_files/logLevelSets.xml", particles, argMap);
    
    EXPECT_EQ(argMap[logLevel], "5")<< "Log level was set to " << argMap[logLevel] << " and not 5" ;
}

TEST(Configuration, testXMLLinkedCell) {
    using namespace io::input;
    std::list<Particle> particles;
    std::unordered_map<io::input::names, std::string> argMap;
    io::input::XMLReader::readFile("../../tests/XML_test_files/simulationStrategySets.xml", particles, argMap);
    
    EXPECT_EQ(argMap[linkedCell], "1")<< "SimulationStrategy was set to " << argMap[linkedCell] << " not LinkedCell" ;
    
    EXPECT_EQ(argMap[boundCondFront], "Reflecting")<< "boundCondFront was set to " << argMap[boundCondFront] << " not Reflecting" ;
    EXPECT_EQ(argMap[boundCondLeft], "Reflecting")<< "boundCondFront was set to " << argMap[boundCondLeft] << " not Reflecting" ;
    EXPECT_EQ(argMap[boundCondRight], "Reflecting")<< "boundCondFront was set to " << argMap[boundCondRight] << " not Reflecting" ;
    EXPECT_EQ(argMap[boundCondRear], "Reflecting")<< "boundCondFront was set to " << argMap[boundCondRear] << " not Reflecting" ;
    EXPECT_EQ(argMap[boundCondTop], "Reflecting")<< "boundCondFront was set to " << argMap[boundCondTop] << " not Reflecting" ;
    EXPECT_EQ(argMap[boundCondBottom], "Reflecting")<< "boundCondFront was set to " << argMap[boundCondBottom] << " not Reflecting" ;
}