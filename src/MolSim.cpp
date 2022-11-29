#include "io/input/sim_input/BodyReader.h"
#include "io/input/cli/CLIArgsParser.h"
#include "io/input/cli/CLIArgs.h"
#include "io/input/arg_names.h"
#include "io/output/Logging.h"
#include "io/IOWrapper.h"
#include "data/ParticleContainer.h"
#include "sim/Simulation.h"
#include "benchmark.h"
#include "defaults.h"

#include <string>
#include <variant>

int main(int argc, char *argsv[]) {
    //Handle input
    io::input::CLIArgsParser parser{argc, argsv};
    parser.parseArgs();

    // get inputFiles
    std::vector<std::string> inputFiles{};
    parser.getInputPaths(inputFiles);

    // get other args
    double dt = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-dt")).value;
    double et = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-et")).value;
    double st = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-st")).value;
    double sig = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-sig")).value;
    bool sigSet = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-sig")).isSet;
    double eps = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-eps")).value;
    bool epsSet = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-eps")).isSet;
    std::string outputBaseName = std::get<io::input::ArgEntry<std::string>>(io::input::cli_arg_map.at("-o")).value;
    std::string outputFolder = std::get<io::input::ArgEntry<std::string>>(io::input::cli_arg_map.at("-of")).value;
    std::string boundCond = std::get<io::input::ArgEntry<std::string>>(io::input::cli_arg_map.at("-bnd")).value;
    bool boundCondSet = std::get<io::input::ArgEntry<std::string>>(io::input::cli_arg_map.at("-bnd")).isSet;
    bool linkedCell = std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-Nlc")).value != 0;
    bool linkedCellSet = std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-Nlc")).isSet;

    // check for benchmark
    bool runBench = std::get<io::input::ArgEntry<std::string>>(io::input::cli_arg_map.at("-bench")).isSet;
    if (runBench) return runBenchmark(dt, et, st, sig, eps, inputFiles, boundCond, linkedCell);

    // no benchmark, run simulation normally

    // check files
    if (inputFiles.empty()) io::input::exitFormatError("No input file specified.");
    // load
    auto ioWrapper = io::IOWrapper<io::input::BodyReader>(inputFiles[0].c_str());
    io::output::loggers::general->info("Loading input file");
    ioWrapper.reload();
    // get file args
    if (!epsSet && ioWrapper.getArgMap().contains(io::input::names::epsilon)) eps = std::stod(ioWrapper.getArgMap().at(io::input::names::epsilon));
    if (!sigSet && ioWrapper.getArgMap().contains(io::input::names::sigma)) sig = std::stod(ioWrapper.getArgMap().at(io::input::names::sigma));
    if (!boundCondSet && ioWrapper.getArgMap().contains(io::input::names::boundCond)) boundCond = ioWrapper.getArgMap().at(io::input::names::boundCond);
    if (!linkedCellSet && ioWrapper.getArgMap().contains(io::input::names::linkedCell)) linkedCell = std::stoi(ioWrapper.getArgMap().at(io::input::names::linkedCell)) != 0;
    // get particles
    std::vector<Particle> buffer;
    ioWrapper.getParticles(buffer);
    ParticleContainer pc = ParticleContainer(buffer);
    buffer.clear();

    //set up simulation
    sim::Simulation<calcF, calcX, calcV> simulation{pc, st, et, dt, eps, sig, outputFolder, outputBaseName, boundCond, linkedCell};
    io::output::loggers::general->info("Initializing simulation");
    simulation.run(
        [&ioWrapper](ParticleContainer &pc,const std::string &outputFolder, const std::string &outputBaseName, int iteration){
            ioWrapper.writeParticlesVTK(pc, outputFolder, outputBaseName, iteration);
        });

    io::output::loggers::general->debug("Output written. Terminating...");

    //clean up
    pc.clear();
    return 0;
}


