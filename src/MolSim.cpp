
#include <boost/program_options.hpp>
#include <iostream>
#include <list>

#include "FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"

/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

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

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char *argsv[]) {
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")(
        "input_file_path,f", boost::program_options::value<std::string>(&inputFilepath), "path to the input file (required)")(
        "delta_t,d", boost::program_options::value<double>(&deltaT)->default_value(0.14),
        "time step per simulation iteration")("end_time,e",
                                              boost::program_options::value<double>(&endTime)->default_value(1000),
                                              "time to stop the simulation");

    boost::program_options::positional_options_description p;
    p.add("input_file_path", -1);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argsv).options(desc).positional(p).run(), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count("input_file_path")) {
        std::cout << "Input file path: " << inputFilepath << std::endl;
    } else {
        std::cout << "Error: no input file path given." << std::endl;
        exit(-1);
    }

    std::cout << "delta_t: " << deltaT << std::endl;
    std::cout << "end_time: " << endTime << std::endl;

    FileReader fileReader;
    fileReader.readFile(particles, inputFilepath);

    double current_time = startTime;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < endTime) {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % 10 == 0) {
            plotParticles(iteration);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += deltaT;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF() {
    std::list<Particle>::iterator iterator;
    iterator = particles.begin();

    for (auto &p1 : particles) {
        std::array<double, 3> f{};
        for (auto &p2 : particles) {
            // @TODO: insert calculation of forces here!
            if (p1 == p2) continue;

            double r = ArrayUtils::L2Norm(p1.getX() - p2.getX());
            f = f + (1 / (r * r * r)) * p1.getM() * p2.getM() * (p2.getX() - p1.getX());
        }

        p1.setOldF(p1.getF());
        p1.setF(f);
    }
}

void calculateX() {
    for (auto &p : particles) {
        // @TODO: insert calculation of position updates here!
        std::array<double, 3> newX = p.getX() + deltaT * p.getV() + (deltaT * deltaT / (2 * p.getM())) * p.getF();
        p.setX(newX);
    }
}

void calculateV() {
    for (auto &p : particles) {
        // @TODO: insert calculation of veclocity updates here!
        std::array<double, 3> newV = p.getV() + (deltaT / (2 * p.getM())) * (p.getF() + p.getOldF());
        p.setV(newV);
    }
}

void plotParticles(int iteration) {
    std::string out_name("MD_vtk");

    outputWriter::VTKWriter writer;
    writer.initializeOutput(particles.size());
    for (auto &p : particles) {
        writer.plotParticle(p);
    }

    writer.writeFile(out_name, iteration);
}
