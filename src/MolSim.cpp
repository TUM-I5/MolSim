#include "outputWriter/VTKWriter.h"
#include "utils/ArgsParser.h"
#include "io/InputLoader.h"
#include "io/FileReader.h"
#include "ParticleContainer.h"
#include "Simulation.h"

#include <iostream>
#include <string>
#include <filesystem>

#define DEFAULT_DELTA_T 0.014
#define DEFAULT_END_TIME 1000
#define DEFAULT_OUTPUT_BASE_NAME "result"
#define DEFAULT_OUTPUT_FOLDER "./output/"

int main(int argc, char *argsv[]) {

    //Handle input
    cli::ArgsParser parser{argc, argsv};
    if (parser.optionArgExists("-dt")) {
        std::string arg = parser.getOptionArg("-dt");
        sim::delta_t = std::stod(arg);
    }
    else sim::delta_t = DEFAULT_DELTA_T;
    if (parser.optionArgExists("-et")) {
        std::string arg = parser.getOptionArg("-et");
        sim::end_time = std::stod(arg);
    }
    else sim::end_time = DEFAULT_END_TIME;
    std::vector<std::string> inputFiles{};
    parser.getInputPaths(inputFiles);
    if (inputFiles.empty()) cli::exitFormatError("No input file specified.");
    std::string outputBaseName {DEFAULT_OUTPUT_BASE_NAME};
    if (parser.optionArgExists("-o")) {
        outputBaseName = parser.getOptionArg("-o");
    }
    std::string outputFolder { DEFAULT_OUTPUT_FOLDER };
    if (parser.optionArgExists("-of")) {
        outputFolder = parser.getOptionArg("-of");
        if (!outputFolder.ends_with("/")) outputFolder = outputFolder.append("/");
    }
    if (!std::filesystem::exists(outputFolder)) std::filesystem::create_directory(outputFolder);
    else if (!std::filesystem::is_directory(outputFolder)) cli::exitFormatError(outputFolder + ": is not a directory!");

    //Load data
    io::InputLoader<const char*, io::FileReader::readFile> inputLoader{inputFiles[0].c_str()};
    inputLoader.reload();
    std::vector<Particle> buffer;
    inputLoader.getParticles(buffer);

    sim::particleContainer = ParticleContainer(buffer);
    buffer.clear();

    //prepare VTK output
    outputWriter::VTKWriter vtkWriter{};

    double current_time = sim::start_time;

    int iteration = 0;

    //set up simulation
    sim::Simulation<> simulation {};

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < sim::end_time) {
        // calculate new x
        simulation.calculateX();
        // calculate new f
        simulation.calculateF();
        // calculate new v
        simulation.calculateV();

        iteration++;
        if (iteration % 10 == 0) {
            vtkWriter.initializeOutput(sim::particleContainer.size());
            for (auto &p: sim::particleContainer) vtkWriter.plotParticle(p);
            vtkWriter.writeFile(outputFolder + outputBaseName, iteration);
        }
        if (iteration % 1000 == 0) {
            std::cout << "Iteration " << iteration << " finished." << std::endl;
        }

        current_time += sim::delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}
