
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include "invsqrt.h"

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

  for (auto &p1 : particles) {
    p1.f = 0.,0.,0.;
    for (auto &p2 : particles) {
      // @TODO: insert calculation of force here!
      if (!(p1 == p2)){
        double delta_x = p1.getX()[0] - p2.getX()[0];
        double delta_y = p1.getX()[1] - p2.getX()[1];
        double scalar = p1.getM() * p2.getM() * std::pow(invsqrtQuake(std::pow(delta_x,2) * std::pow(delta_y, 2)), 3);
        double F_X = -delta_x / scalar;
        double F_Y = -delta_y / scalar;
        p1.f[0] += F_X;
        p1.f[1] += F_Y;
      }
    }
  }
}

void calculateX() {
  for (auto &p : particles) {
    // @TODO: insert calculation of force here!
  }
}

void calculateV() {
  for (auto &p : particles) {
    // @TODO: insert calculation of force here!
  }
}

void plotParticles(int iteration) {

  std::string out_name("MD_vtk");

  outputWriter::XYZWriter writer;
  writer.plotParticles(particles, out_name, iteration);
}
