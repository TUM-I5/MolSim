
#include "./inputReader/FileReader.h"
#include "./model/ParticleContainer.h"
#include "./simulation/GravitySimulation.h"

#include <iostream>
#include <list>
#include <memory>

double end_time;
double delta_t;

ParticleContainer particleContainer;

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 4) {
    std::cout << "Erroneous program call! " << std::endl;
    std::cout << "Usage: ./MolSim filename t_end delta_t" << std::endl;
    return EXIT_FAILURE;
  }

  end_time = std::__cxx11::stod(argsv[2]);
  delta_t = std::__cxx11::stod(argsv[3]);

  // creating a new InputReader which in this case is the FileReader provided
  std::unique_ptr<InputReader> inputReader = std::make_unique<FileReader>();
  inputReader->readInput(particleContainer, argsv[1]);
  
  // initializing the GravitySimulation which calculates forces according with assignment 1
  std::unique_ptr<Simulation> simulation = std::make_unique<GravitySimulation>(GravitySimulation(&particleContainer, end_time, delta_t)); 
  simulation->simulate(); 

  return EXIT_SUCCESS;
}
