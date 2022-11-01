
#include "FileReader.h"
#include "outputWriter/OutputFacade.h"
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
 * plot the particles to a vtk-file
 */
void plotParticles(int iteration);

constexpr double start_time = 0;
double end_time;
double delta_t;

// TODO: what data structure to pick?
std::list<Particle> particles;

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 4) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "Usage: ./MolSim filename t_end delta_t" << std::endl;
  }

  FileReader fileReader;
  fileReader.readFile(particles, argsv[1]);

  // Initialize output
  OutputFacade* output = new OutputFacade(&particles);

  end_time = std::__cxx11::stod(argsv[2]);
  delta_t = std::__cxx11::stod(argsv[3]);

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
    if (iteration % 10 == 0) { //Original mod 10, just for testing
      output->outputVTK(iteration);
      output->outputXYZ(iteration);
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
    //std::cout << "calcF: outer for loop" << std::endl; //Test
    
    std::array<double,3> f_i = {0.0 , 0.0, 0.0};
    p1.setOldF(p1.getF());

    for (auto &p2 : particles) {
      //std::cout << "calcF: inner for loop" << std::endl; //Test
      
      if (!p1.operator==(p2)) {

        double distance = ArrayUtils::L2Norm(p1.getX() - p2.getX());
        //std::cout << "Distance: " << distance << std::endl;

        std::array<double,3> f_ij = (p1.getM() * p2.getM() / pow(distance,3)) * (p2.getX() - p1.getX());
        
        f_i = f_i + f_ij;
        //std::cout << "F_i: " << ArrayUtils::to_string(f_i) << std::endl;
      }
    }
    p1.setF(f_i);
  }
}

void calculateX() {
  for (auto &p : particles) {
    // @TODO: insert calculation of position updates here!
    std::array<double,3> x_new = p.getX() + delta_t * p.getV() + (delta_t * delta_t / (2 * p.getM()))* p.getF();
    //std::cout << "New x: " << ArrayUtils::to_string(x_new) << std::endl;
    p.setX(x_new);
  }
}

void calculateV() {
  for (auto &p : particles) {
    // @TODO: insert calculation of veclocity updates here!
    std::array<double, 3> v_new = p.getV() + (delta_t / (2*p.getM())) * (p.getOldF() + p.getF());
    //std::cout << "New v: " << ArrayUtils::to_string(v_new) << std::endl;
    p.setV(v_new);
  }
}
