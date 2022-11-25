#include "io/input/cli/CLIArgsParser.h"
#include "io/IOWrapper.h"
#include "data/ParticleContainer.h"
#include "io/output/Logging.h"
#include "io/input/sim_input/BodyReader.h"
#include "data/Body.h"
#include "Simulation.h"
#include "defaults.h"

#include <string>
#include <filesystem>

static constexpr auto calcF = sim::calculateFLennardJonesFast;
static constexpr auto calcX = sim::calculateXStoermerVelvetFast;
static constexpr auto calcV = sim::calculateVStoermerVelvetFast;

template <typename V>
struct ArgEntry {
    std::string shortName;
    std::string longName;
    bool expectParam;
    std::function<V(std::string&)> extractor;
    std::function<void(std::string&)> handler;
};

int main(int argc, char *argsv[]) {
    //Handle input
    io::input::CLIArgsParser parser{argc, argsv};

    // Setup logger
    if (parser.optionArgExists("-llv")) {
        std::string arg = parser.getOptionArg("-llv");
        int lv = std::stoi(arg);
        if (lv >= io::output::loggers::level::level_count) lv = io::output::loggers::level_count - 1;
        else if (lv < 0) lv = 0;
        io::output::loggers::init(static_cast<io::output::loggers::level>(lv));
    }
    else io::output::loggers::init(io::output::loggers::level::info);

    // get deltaTime
    double dt;
    if (parser.optionArgExists("-dt")) {
        std::string arg = parser.getOptionArg("-dt");
        dt = std::stod(arg);
    }
    else dt = default_delta_t;

    // get endTime
    double et;
    if (parser.optionArgExists("-et")) {
        std::string arg = parser.getOptionArg("-et");
        et = std::stod(arg);
    }
    else et = default_end_time;

    // get startTime
    double st;
    if (parser.optionArgExists("-st")) {
        std::string arg = parser.getOptionArg("-st");
        st = std::stod(arg);
    }
    else st = default_start_time;

    // get Sigma
    double sig;
    bool sigOverride = false;
    if (parser.optionArgExists("-sig")) {
        std::string arg = parser.getOptionArg("-sig");
        sig = std::stod(arg);
        sigOverride = true;
    }
    else sig = default_sigma;

    // get Epsilon
    double eps;
    bool epsOverride = false;
    if (parser.optionArgExists("-eps")) {
        std::string arg = parser.getOptionArg("-eps");
        eps = std::stod(arg);
        epsOverride = true;
    }
    else eps = default_epsilon;

    // get inputFiles
    std::vector<std::string> inputFiles{};
    parser.getInputPaths(inputFiles);

    // get OutputBaseName
    std::string outputBaseName {default_output_base_name};
    if (parser.optionArgExists("-o")) {
        outputBaseName = parser.getOptionArg("-o");
    }

    // get OutputFolder name and create if necessary
    std::string outputFolder { default_output_folder };
    if (parser.optionArgExists("-of")) {
        outputFolder = parser.getOptionArg("-of");
        if (!outputFolder.ends_with("/")) outputFolder = outputFolder.append("/");
    }
    if (!std::filesystem::exists(outputFolder)) std::filesystem::create_directory(outputFolder);
    else if (!std::filesystem::is_directory(outputFolder)) io::input::exitFormatError(outputFolder + ": is not a directory!");

    // handle benchmark request
    if (parser.optionArgExists("-bench")) {
        int iterations;
        if (!parser.optionArgExists("-i")) iterations = default_bench_iterations;
        else {
            std::string tmp = parser.getOptionArg("-i");
            iterations = std::stoi(tmp);
        }

        std::string type = parser.getOptionArg("-bench");
        if (type == "default") {
            // get max body size
            int maxBodySize;
            if (parser.optionArgExists("-bMax")) {
                std::string tmp = parser.getOptionArg("-bMax");
                maxBodySize = std::stoi(tmp);
            } else maxBodySize = default_bench_maxBody;

            // generate 2 bodies in varying sizes
            struct Body b0 { Shape::cuboid, {-1.1, 0, 0}, {10, 10, 1}, 0.1, 0.1, {0.001, 0, 0} };
            struct Body b1 { Shape::cuboid, { 0.1, 0, 0}, {10, 10, 1}, 0.1, 0.1, {0.001, 0, 0} };
            std::list<Particle> buffer_tmp;
            std::vector<Particle> buffer;
            for(int bSize { 1 }; bSize <= maxBodySize; bSize *= 2) {
                b0.dimensions[2] = bSize;
                b1.dimensions[2] = bSize;

                ParticleGenerator::generateCuboid(b0, 0.1, buffer_tmp, 3);
                ParticleGenerator::generateCuboid(b1, 0.1, buffer_tmp, 3);
                for(const auto& p : buffer_tmp) buffer.push_back(p);
                sim::Simulation<calcF, calcX, calcV> simulation {st, et, dt, eps, sig, outputFolder, outputBaseName};
                simulation.runBenchmark(iterations, "default", buffer);

                buffer_tmp.clear();
                buffer.clear();
            }

        }
        else if (type == "file") {
            std::vector<Particle> buffer;
            for(const auto& file : inputFiles) {
                auto iow = io::IOWrapper<io::input::BodyReader>(file.c_str());
                iow.reload();
                iow.getParticles(buffer);
                if (!epsOverride) eps = iow.getEpsilon();
                if (!sigOverride) sig = iow.getSigma();

                sim::Simulation<calcF, calcX, calcV> simulation {st, et, dt, eps, sig, outputFolder, outputBaseName};
                simulation.runBenchmark(iterations, file, buffer);
                buffer.clear();
            }
        }
        else io::input::exitFormatError(type + ": is an unknown benchmark input type!");
        return 0;
    }

    // no benchmark, run simulation normally
    // Load data
    if (inputFiles.empty()) io::input::exitFormatError("No input file specified.");
    auto ioWrapper = io::IOWrapper<io::input::BodyReader>(inputFiles[0].c_str());
    io::output::loggers::general->info("Loading input file");
    ioWrapper.reload();
    std::vector<Particle> buffer;
    ioWrapper.getParticles(buffer);
    if (!epsOverride) eps = io::ioWrapper->getEpsilon();
    if (!sigOverride) sig = io::ioWrapper->getSigma();

    sim::particleContainer = ParticleContainer(buffer);
    buffer.clear();

    //set up simulation
    sim::Simulation<calcF, calcX, calcV> simulation {st, et, dt, eps, sig, outputFolder, outputBaseName};
    io::output::loggers::general->info("Initializing simulation");
    simulation.run();

    io::output::loggers::general->debug("Output written. Terminating...");
    sim::particleContainer.clear();
    return 0;
}
