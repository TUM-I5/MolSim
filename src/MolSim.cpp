
#include "FileReader.h"
#include "utils/ArrayUtils.h"
#include "ParticleContainer.h"
#include "ParticleGenerator.h"
#include "Formulas.h"

#include <iostream>
#include <vector>
#include "outputWriter/VTKWriter.h"
#include <spdlog/spdlog.h>


/**** forward declaration of the calculation functions ****/

/**
 * calculate the force for all particles
 */
void calculateF();

/**
 * calculate the position for all particles
 */
void calculateX(double delta_t);

/**
 * calculate the position for all particles
 */
void calculateV(double delta_t);

/**
 * plot the particles to a xyz-file
 */
void plotParticles(int iteration);

constexpr double start_time = 0;

//ParticleContainer particles;
std::vector<ParticleGenerator> genList;
int main(int argc, char *argsv[]) {

    //test
    spdlog::info("Application started");
    spdlog::info("Hello from MolSim for PSE!");

    if (argc != 2) {
         spdlog::error("Erroneous programme call! ");
         spdlog::error("./molsym filename");
    }

    FileReader fileReader;

    fileReader.readFile(genList, argsv[1]);

    //passing arguments via the command line
    double end_time = std::atof(argsv[2]);
    double delta_t = std::atof(argsv[3]);

    double current_time = start_time;

    int iteration = 0;

    /*
     * You should calculate forces once before the simulation loop starts -
     * else your first updates on positions and velocities use and old Force of 0
     */

    //put all particles in one container
    for (Particle particle : genList[1].getParticleContainer().getParticles()) {
        genList[0].getParticleContainer().addParticle(particle);
    }

    //create the particle pairs
    genList[0].getParticleContainer().createParticlePairs();

    calculateX(delta_t);

    for (auto &p1:  genList[0].getParticleContainer().getParticlePairs()) {
        p1.first.setF(Formulas::calculateLJForce(const_cast<std::array<double, 3> &>(p1.first.getX()),
                                                 const_cast<std::array<double, 3> &>(p1.second.getX()), 1, 5));
        p1.second.setF(Formulas::calculateLJForce(const_cast<std::array<double, 3> &>(p1.second.getX()),
                                                  const_cast<std::array<double, 3> &>(p1.second.getX()), 1, 5));
    }

    calculateV(delta_t);

    if (iteration % 10 == 0) {
        plotParticles(iteration);
    }

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        // calculate new x
        calculateX(delta_t);
        // calculate new f
        for (auto &p1:  genList[0].getParticleContainer().getParticlePairs()) {
            p1.first.setF(Formulas::calculateLJForce(const_cast<std::array<double, 3> &>(p1.first.getX()),
                                       const_cast<std::array<double, 3> &>(p1.second.getX()), 1, 5));
            p1.second.setF(Formulas::calculateLJForce(const_cast<std::array<double, 3> &>(p1.second.getX()),
                                                     const_cast<std::array<double, 3> &>(p1.first.getX()), 1, 5));
        }
        // calculate new v
        calculateV(delta_t);

        iteration++;
        if (iteration % 10 == 0) {
            plotParticles(iteration);
        }
        spdlog::info("Iteration {} finished.", iteration);

        current_time += delta_t;

    }

    //Logger::getLogger()->info("output written. Terminating...");
    return 0;
}


void calculateF() {

    for (auto &p1: genList[0].getParticleContainer().getParticles()) {
        std::array<double, 3> F_i{0., 0., 0.};
        for (auto &p2: genList[0].getParticleContainer().getParticles()) {
            // formula: Fij = ((mi * mj) / ||xi −xj||^3) * (xj − xi)
            std::array<double, 3> F_ij{};
            if (&p1 != &p2) {
                auto mul = p1.getM() * p2.getM() * (p2.getX() - p1.getX());

                for (int i = 0; i < 3; ++i) {
                    F_ij[i] = mul[i] / pow(Formulas::secondNorm((p1.getX() - p2.getX())), 3.0);
                    F_i[i] += F_ij[i];
                }
            }
        }
        p1.setF(F_i);
    }
}

void calculateX(double delta_t) {
    for (auto &p: genList[0].getParticleContainer().getParticles()) {
        // formula: xi(tn+1) = xi(tn) + ∆t · vi(tn) + (∆t)^2 * (Fi(tn)/2mi)
        // Calculate xi(tn+1)

        auto xi_tn1 = p.getX() + delta_t * p.getV() + (delta_t * delta_t) / (2.0 * p.getM()) * p.getF();

        p.setX(xi_tn1);  // Update the position
    }
}

void calculateV(double delta_t) {
    for (auto &p: genList[0].getParticleContainer().getParticles()) {
        // formula: vi(tn+1) = vi(tn) + ∆t * ((Fi(tn) + Fi(tn+1))/ 2mi)
        // Calculate the velocity at time tn+1

        auto vi_tn1 = p.getV() + delta_t / (2 * p.getM()) * (p.getOldF() + p.getF());
        p.setV(vi_tn1);
    }
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::VTKWriter writer;
    writer.initializeOutput(genList[0].getParticleContainer().size());
    for (auto &p: genList[0].getParticleContainer().getParticles()) {
        writer.plotParticle(p);
    }
    writer.writeFile(out_name, iteration);
}
