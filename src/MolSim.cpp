
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>

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

constexpr double start_time = 0;
constexpr double end_time = 1000;
constexpr double delta_t = 0.014;

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char *argsv[]) {

    std::cout << "Hello from MolSim for PSE!" << std::endl;
    if (argc != 2) {
        std::cout << "Erroneous programme call! " << std::endl;
        std::cout << "./molsym filename" << std::endl;
    }

    FileReader fileReader;
    fileReader.readFile(particles, argsv[1]);

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
            plotParticles(iteration);
        }
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF() {
    std::list<Particle>::iterator iterator;
    iterator = particles.begin();

    for (auto &p1: particles) {
        std::array<double, 3> forceX1{};
        for (auto &p2: particles) {
            // @TODO: insert calculation of forces here!
            // formula: Fij = ((mi * mj) / ||xi −xj||^3) * (xj − xi)
            std::array<double, 3> fo{};
            auto force = p1.getM() * p2.getM() * (p2.getX() - p1.getX());

            for (int i = 0; i < 3; ++i) {
                fo[i] += force[i] / pow(sqrt(pow(p1.getX()[i] - p2.getX()[i], 2.0)), 3.0);
                forceX1[i] += fo[i];
            }
        }
        p1.setOldF(p1.getF());
        p1.setF(forceX1);
    }
}

void calculateX() {
    for (auto &p: particles) {
        // @TODO: insert calculation of position updates here!
        // Calculate xi(tn+1)
        // formula: xi(tn+1) = xi(tn) + ∆t · vi(tn) + (∆t)^2 * (Fi(tn)/2mi)

        auto xi_tn1 = p.getX() + delta_t * p.getV() + (delta_t * delta_t) / (2.0 * p.getM()) * p.getOldF();

        p.setX(xi_tn1);  // Update the position
    }
}

void calculateV() {
    for (auto &p: particles) {
        // @TODO: insert calculation of veclocity updates here!
        // formula: vi(tn+1) = vi(tn) + ∆t * ((Fi(tn) + Fi(tn+1))/ 2mi)
        // Calculate the velocity at time tn+1

        auto vi_tn1 = p.getV() + delta_t / (2 * p.getM()) * (p.getOldF() + p.getF());
        p.setV(vi_tn1);
    }
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticles(particles, out_name, iteration);
}
