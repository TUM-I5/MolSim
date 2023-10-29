
#include "FileReader.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>
#include <cmath>

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
double end_time; // 1000
double delta_t; // 0.014

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
    if (argc != 4) {
        std::cout << "Erroneous programme call! " << '\n';
        std::cout << "./MolSim <input filename> <end time> <time delta>" << std::endl;
        return 1;
    }

    char *end;
    end_time = std::strtod(argsv[2], &end);

    if (*end) {
        std::cout << "Invalid end time: " << argsv[2] << std::endl;
        return 1;
    }

    delta_t = std::strtod(argsv[3], &end);

    if (*end) {
        std::cout << "Invalid time delta: " << argsv[3] << std::endl;
        return 1;
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
        std::array<double, 3> newF = {0., 0., 0.};
        for (auto &p2: particles) {
            if (p1 == p2) {
                // particles are the same. skip.
                continue;
            }

            const auto force =
                    (p1.getM() * p2.getM() / std::pow(p2.distanceTo(p1), 3)) * (p2.getX() - p1.getX());

            newF = newF + force;
        }

        p1.updateF(newF);
    }
}

void calculateX() {
  for (auto &p : particles) {
      auto newX =
              p.getX() +
              delta_t * p.getV() +
              (delta_t * delta_t / (2 * p.getM())) * p.getF();

      p.setX(newX);
  }
}

void calculateV() {
  for (auto &p : particles) {
      auto newV =
              p.getV() +
              (delta_t / (2 * p.getM())) * (p.getOldF() + p.getF());

      p.setV(newV);
  }
}

void plotParticles(int iteration) {

    std::string out_name("output/MD_vtk");

    outputWriter::VTKWriter writer;

    writer.initializeOutput(particles.size());


    for (auto &particle: particles) {
        writer.plotParticle(particle);
    }

    writer.writeFile(out_name, iteration);
}
