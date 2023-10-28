
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"
#include "ParticleContainer.h"

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

void shiftForces();

constexpr double start_time = 0;
constexpr double end_time = 4000;
constexpr double delta_t = 0.014;


ParticleContainer particles;
outputWriter::VTKWriter writer;

int main(int argc, char *argsv[]) {

  std::cout << "Hello from MolSim for PSE!" << std::endl;
  if (argc != 2) {
    std::cout << "Erroneous programme call! " << std::endl;
    std::cout << "./molsym filename" << std::endl;
  }

  std::list<Particle> particles_list;
  FileReader fileReader;
  fileReader.readFile(particles_list, argsv[1]);
  particles = ParticleContainer(particles_list);
  writer.initializeOutput(particles.size());

  double current_time = start_time;

  int iteration = 0;

  //calculate inital force:
  std::cout << "calculate initial force" << std::endl;
  calculateF();
  shiftForces();

    for(auto &p : particles){
        std::cout << p << std::endl;
    }
  std::cout << "done" << std::endl;


  // for this loop, we assume: current x, current f and current v are known
  while (current_time < end_time) {


      //std::cout << "calc X" << std::endl;
    // calculate new x
    calculateX();
     // std::cout << "calc F" << std::endl;
    // calculate new f
    calculateF();
     // std::cout << "calc V" << std::endl;
    // calculate new v
    calculateV();

    iteration++;
    //plotParticles(iteration);
    if (iteration % 10 == 0) {
        writer.initializeOutput(particles.size());
        for(auto &p : particles){
            writer.plotParticle(p);
        }
        writer.writeFile("out",iteration);

    }
    //std::cout << "calc shift Forces" << std::endl;
    shiftForces();
    std::cout << "Iteration " << iteration << " finished." << std::endl;

    current_time += delta_t;
  }

  std::cout << "output written. Terminating..." << std::endl;
  return 0;
}



/**
 *
 * @brief calculates the force of every Particle at the current time step
 *
 *
 *
 * @param None
 * @return None
 *
 */
void calculateF() {



  const auto amt_particles = particles.size();
  using Matrix = std::vector<std::vector<std::array<double,3>>>;
  Matrix forces(amt_particles, std::vector<std::array<double, 3>>(amt_particles));


  for(size_t i = 0; i < amt_particles; i++){
      //std::cout << "i=" << i <<  std::endl;
      for(size_t j = 0; j <amt_particles; j++){
          if(i!=j) {
              double  m_i =  particles[i].getM();
              double m_j =  particles[j].getM();
              std::array<double,3> x_i =  particles[i].getX();
              std::array<double,3> x_j = particles[j].getX();
              auto diff = x_j - x_i;
              double norm = ArrayUtils::L2Norm(diff);
              forces[i][j] = ( (m_i * m_j) / std::pow(norm,3)  ) * diff;
              //std::cout << forces[i][j]  << "\n" << std::endl;
          }
      }
  }
  for(size_t i = 0; i < amt_particles; i++){
      std::array<double,3> F_i{0.0,0.0,0.0};
      for(size_t j = 0; j < amt_particles ; j++){
          if(i != j)
            F_i = F_i + forces[i][j];
      }
      //std::cout << F_i  << "\n" << std::endl;
      particles[i].setF(0,F_i[0]);
      particles[i].setF(1,F_i[1]);
      particles[i].setF(2,F_i[2]);
  }
}

void calculateX() {
  for (auto &p : particles) {
      const std::array<double, 3> &old_x = p.getX();
      const std::array<double, 3> &v = p.getV();
      const std::array<double, 3> &f = p.getOldF();
      const double &m=p.getM();
      for (int i=0;i<3;i++){
          p.setX(i,old_x[i]+delta_t*v[i]+delta_t*delta_t*f[i]/2.0/m);
      }
  }
}

void calculateV() {
  for (auto &p : particles) {

      const std::array<double, 3> &v = p.getV();
      const std::array<double, 3> &f = p.getF();
      const std::array<double, 3> &f_old = p.getOldF();
      const double &m = p.getM();

      for(int i=0; i < v.size(); i++) {
          p.setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
      }
  }
}


void shiftForces(){
    for(auto &p : particles){
        auto old_F = p.getF();
        p.setOldF(0,old_F[0]);
        p.setOldF(1,old_F[1]);
        p.setOldF(2,old_F[2]);
    }
}

void plotParticles(int iteration) {

  std::string out_name("MD_vtk");

  outputWriter::XYZWriter writer;
  writer.plotParticles(particles, out_name, iteration);
}


