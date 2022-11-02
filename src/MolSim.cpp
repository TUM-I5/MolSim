
#include "./inputReader/FileReader.h"
#include "./model/ParticleContainer.h"
#include "./simulation/GravitySimulation.h"

#include <iostream>
#include <list>

double end_time;
double delta_t;

ParticleContainer particleContainer;

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 4) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "Usage: ./MolSim filename t_end delta_t" << std::endl;
  }

  end_time = std::__cxx11::stod(argsv[2]);
  delta_t = std::__cxx11::stod(argsv[3]);

  // creating a new InputReader which in this case is the FileReader provided
  InputReader *inputReader = new FileReader();
  inputReader->readInput(particleContainer, argsv[1]);
  
  // initializing the GravitySimulation which calculates forces according with assignment 1
  Simulation *simulation = new GravitySimulation(particleContainer, end_time, delta_t); 
  simulation->simulate(); 

  return 0;
}
