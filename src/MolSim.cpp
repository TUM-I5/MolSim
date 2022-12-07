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
    if (config.get<benchmark>()) return runBenchmark(config, inputFiles, parser.getLoader());

    // no benchmark, run simulation normally
    // check files
    if (inputFiles.empty()) io::input::exitFormatError("No input file specified.");

    // load all input files
    std::vector<Particle> buffer;
    auto ioWrapper = io::IOWrapper(parser.getLoader());
    for (auto& file : inputFiles) {
        ioWrapper.setLocator(file.c_str());
        io::output::loggers::general->info("Loading input file: {}", file);
        ioWrapper.reload();
        // get particles from current file
        ioWrapper.getParticles(buffer);
    }
    // get final file args
    config.loadIOWArgs(ioWrapper.getArgMap());

    ParticleContainer pc = ParticleContainer(buffer,
                                             {config.get<boundingBox_X0>(), config.get<boundingBox_X1>(), config.get<boundingBox_X2>()},
                                             config.get<rCutoff>());
    buffer.clear();

    //set up simulation
    sim::Simulation simulation{ioWrapper, pc, config};
    io::output::loggers::general->info("Initializing simulation");
    simulation.run(config);

    io::output::loggers::general->debug("Output written. Terminating...");

    //clean up
    pc.clear();
    return 0;
}


