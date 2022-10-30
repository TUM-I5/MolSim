
#include "MolSim.h"
#include "utils/ArgsParser.h"
#include "io/InputLoader.h"
#include "io/FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"
#include "ParticleContainer.h"

#include <Eigen>
#include <iostream>
#include <string>
#include <filesystem>

constexpr double start_time = 0;
double end_time;
double delta_t;
#define DEFAULT_DELTA_T 0.014
#define DEFAULT_END_TIME 1000
#define DEFAULT_OUTPUT_BASE_NAME "result"
#define DEFAULT_OUTPUT_FOLDER "./output/"

ParticleContainer particle_wrapper;

int main(int argc, char *argsv[]) {

    //Handle input
    cli::ArgsParser parser{argc, argsv};
    if (parser.optionArgExists("-dt")) {
        std::string arg = parser.getOptionArg("-dt");
        delta_t = std::stod(arg);
    }
    else delta_t = DEFAULT_DELTA_T;
    if (parser.optionArgExists("-et")) {
        std::string arg = parser.getOptionArg("-et");
        end_time = std::stod(arg);
    }
    else end_time = DEFAULT_END_TIME;
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

    particle_wrapper = ParticleContainer(buffer);
    buffer.clear();

    //prepare VTK output
    outputWriter::VTKWriter vtkWriter{};

    double current_time = start_time;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % 10 == 0) {
            vtkWriter.initializeOutput(particle_wrapper.size());
            for (auto &p: particle_wrapper) vtkWriter.plotParticle(p);
            vtkWriter.writeFile(outputFolder + outputBaseName, iteration);
        }
        if (iteration % 1000 == 0) {
            std::cout << "Iteration " << iteration << " finished." << std::endl;
        }

        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF() {
    //set all current forces on all particles to 0
    particle_wrapper.forAllParticles([](Particle &p) {
        p.setOldF(p.getF());
        p.setF({0., 0., 0.});
    });

    particle_wrapper.forAllPairs(forceBetw2Particles);
}

void calculateX() {
    particle_wrapper.forAllParticles([](Particle &p) {
        Eigen::Vector3d x = delta_t * p.getV() + delta_t * delta_t * p.getOldF() / (2 * p.getM());
        p.add_to_X(x);
    });
}

void calculateV() {
    particle_wrapper.forAllParticles([](Particle &p) {
        Eigen::Vector3d v = delta_t * (p.getOldF() + p.getF()) / (2 * p.getM());
        p.add_to_V(v);
    });
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticles(particle_wrapper, out_name, iteration);
}

void forceBetw2Particles(Particle &p1, Particle &p2) {
    double delta_x = p1.getX()[0] - p2.getX()[0];
    double delta_y = p1.getX()[1] - p2.getX()[1];
    double scalar =
            p1.getM() * p2.getM() * std::pow(1 / std::sqrt(delta_x * delta_x + delta_y * delta_y), 3);
    double F_X = -delta_x * scalar;
    double F_Y = -delta_y * scalar;
    p1.add_to_F({F_X, F_Y, 0.});
    p2.add_to_F({-F_X, -F_Y, 0.});
}