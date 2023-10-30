
#include <boost/program_options.hpp>
#include <iostream>
#include <list>

#include "FileReader.h"
#include "GravityCalculation.h"
#include "ParticleContainer.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"

/**** forward declaration of the calculation functions ****/

/**
 * calculate the position for all particles
 */
void calculateX();

/**
 * calculate the position for all particles
 */
void calculateV();

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

std::string inputFilepath;
constexpr double startTime = 0;
double endTime;
double deltaT;

ParticleContainer particleContainer;
ForceSource &&gravitational_force = GravitationalForce();

int main(int argc, char *argsv[]) {
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")(
        "input_file_path,f", boost::program_options::value<std::string>(&inputFilepath),
        "The path to the input file. Must be specified, otherwise the program will terminate. Can be inserted as "
        "positional argument.")("delta_t,d", boost::program_options::value<double>(&deltaT)->default_value(0.014),
                                "The time step per simulation iteration")(
        "end_time,e", boost::program_options::value<double>(&endTime)->default_value(1000),
        "The time, at which the simulation will end");

    boost::program_options::positional_options_description p;
    p.add("input_file_path", -1);

    boost::program_options::variables_map vm;
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argsv).options(desc).positional(p).run(), vm);
    boost::program_options::notify(vm);

    if (argc <= 1 || vm.count("help")) {
        std::cout << desc << std::endl;
        return -1;
    }

    if (!vm.count("input_file_path")) {
        std::cout << "Error: no input file path given." << std::endl;
        std::cout << desc << std::endl;
        return -1;
    }

    FileReader fileReader;
    fileReader.readFile(particleContainer, inputFilepath);

    double current_time = startTime;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < endTime) {
        // calculate new x
        calculateX();
        // calculate new f
        particleContainer.resetForces();
        particleContainer.applyForceSource(gravitational_force);
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % 50 == 0) {
            plotParticles(iteration);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += deltaT;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateX() {
    for (auto &p : particleContainer) {
        std::array<double, 3> newX = p.getX() + deltaT * p.getV() + (deltaT * deltaT / (2 * p.getM())) * p.getF();
        p.setX(newX);
    }
}

void calculateV() {
    for (auto &p : particleContainer) {
        std::array<double, 3> newV = p.getV() + (deltaT / (2 * p.getM())) * (p.getF() + p.getOldF());
        p.setV(newV);
    }
}

void plotParticles(int iteration) {
    std::string out_name("MD_vtk");

    outputWriter::VTKWriter writer;
    writer.initializeOutput(particleContainer.size());
    for (auto &p : particleContainer) {
        writer.plotParticle(p);
    }

    writer.writeFile(out_name, iteration);
}
