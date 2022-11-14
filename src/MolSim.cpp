#include "io/ArgsParser.h"
#include "io/IOWrapper.h"
#include "data/ParticleContainer.h"
#include "io/Logging.h"
#include "io/BodyReader.h"
#include "Simulation.h"
#include "defaults.h"

#include <string>
#include <filesystem>

int main(int argc, char *argsv[]) {
    //Handle input
    cli::ArgsParser parser{argc, argsv};

    // Setup logger
    if (parser.optionArgExists("-llv")) {
        std::string arg = parser.getOptionArg("-llv");
        int lv = std::stoi(arg);
        if (lv >= loggers::level::level_count) lv = loggers::level_count - 1;
        else if (lv < 0) lv = 0;
        loggers::init(static_cast<loggers::level>(lv));
    }
    else loggers::init(loggers::level::info);
    double dt;
    if (parser.optionArgExists("-dt")) {
        std::string arg = parser.getOptionArg("-dt");
        dt = std::stod(arg);
    }
    else dt = default_delta_t;
    double et;
    if (parser.optionArgExists("-et")) {
        std::string arg = parser.getOptionArg("-et");
        et = std::stod(arg);
    }
    else et = default_end_time;
    double st;
    if (parser.optionArgExists("-st")) {
        std::string arg = parser.getOptionArg("-st");
        st = std::stod(arg);
    }
    else st = default_start_time;
    double sig;
    bool sigOverride = false;
    if (parser.optionArgExists("-sig")) {
        std::string arg = parser.getOptionArg("-sig");
        sig = std::stod(arg);
        sigOverride = true;
    }
    else sig = default_sigma;
    double eps;
    bool epsOverride = false;
    if (parser.optionArgExists("-eps")) {
        std::string arg = parser.getOptionArg("-eps");
        eps = std::stod(arg);
        epsOverride = true;
    }
    else eps = default_epsilon;
    std::vector<std::string> inputFiles{};
    parser.getInputPaths(inputFiles);
    if (inputFiles.empty()) cli::exitFormatError("No input file specified.");
    std::string outputBaseName {default_output_base_name};
    if (parser.optionArgExists("-o")) {
        outputBaseName = parser.getOptionArg("-o");
    }
    std::string outputFolder { default_output_folder };
    if (parser.optionArgExists("-of")) {
        outputFolder = parser.getOptionArg("-of");
        if (!outputFolder.ends_with("/")) outputFolder = outputFolder.append("/");
    }
    if (!std::filesystem::exists(outputFolder)) std::filesystem::create_directory(outputFolder);
    else if (!std::filesystem::is_directory(outputFolder)) cli::exitFormatError(outputFolder + ": is not a directory!");

    //Load data
    io::ioWrapper = std::make_shared<io::IOWrapper<io::BodyReader>>(inputFiles[0].c_str());
    io::ioWrapper->reload();
    std::vector<Particle> buffer;
    io::ioWrapper->getParticles(buffer);
    if (!epsOverride) eps = io::ioWrapper->getEpsilon();
    if (!sigOverride) sig = io::ioWrapper->getSigma();

    sim::particleContainer = ParticleContainer(buffer);
    buffer.clear();

    //set up simulation
    sim::Simulation<> simulation {st, et, dt, eps, sig, outputFolder, outputBaseName};
    simulation.run();

    loggers::general->debug("Output written. Terminating...");
    return 0;
}
