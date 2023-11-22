
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


constexpr double start_time = 0;

//ParticleContainer particles;
std::vector<ParticleGenerator> genList;
int main(int argc, char *argsv[]) {

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

    Formulas::calculateX(delta_t, genList[0].getParticleContainer());

    for (auto &p1:  genList[0].getParticleContainer().getParticlePairs()) {
        Formulas::calculateLJForce(p1, 1, 5);
    }

    Formulas::calculateV(delta_t, genList[0].getParticleContainer());

    if (iteration % 10 == 0) {
        Formulas::plotParticles(iteration, genList[0].getParticleContainer());
    }

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        // calculate new x
        Formulas::calculateX(delta_t, genList[0].getParticleContainer());
        // calculate new f
        for (auto &p1:  genList[0].getParticleContainer().getParticlePairs()) {
            (Formulas::calculateLJForce(p1, 1, 5));
        }
        // calculate new v
        Formulas::calculateV(delta_t, genList[0].getParticleContainer());

        iteration++;
        if (iteration % 10 == 0) {
            Formulas::plotParticles(iteration, genList[0].getParticleContainer());
        }
        spdlog::info("Iteration {} finished.", iteration);

        current_time += delta_t;

    }

    spdlog::info("output written. Terminating...");
    return 0;
}
