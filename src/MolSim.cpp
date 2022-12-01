#include "io/input/sim_input/BodyReader.h"
#include "io/input/cli/CLIArgsParser.h"
#include "io/input/arg_names.h"
#include "io/output/Logging.h"
#include "io/IOWrapper.h"
#include "data/ParticleContainer.h"
#include "sim/Simulation.h"
#include "benchmark.h"
#include "io/input/Configuration.h"

#include <string>
#include <variant>

using namespace io::input;

int main(int argc, char *argsv[]) {
    //Handle input
    CLIArgsParser parser{argc, argsv};
    parser.parseArgs();

    // get inputFiles
    std::vector<std::string> inputFiles{};
    parser.getInputPaths(inputFiles);

    // get other args
    Configuration config{};
    config.loadCLIArgs();

    // check for benchmark
    if (config.get<benchmark>()) return runBenchmark(config, inputFiles);

    // no benchmark, run simulation normally
    // check files
    if (inputFiles.empty()) io::input::exitFormatError("No input file specified.");
    // load
    auto ioWrapper = io::IOWrapper<io::input::BodyReader>(inputFiles[0].c_str());
    io::output::loggers::general->info("Loading input file");
    ioWrapper.reload();
    // get file args
    config.loadIOWArgs(ioWrapper.getArgMap());
    // get particles
    std::vector<Particle> buffer;
    ioWrapper.getParticles(buffer);
    ParticleContainer pc = ParticleContainer(buffer);
    buffer.clear();

    //set up simulation
    sim::Simulation simulation{pc, config};
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


