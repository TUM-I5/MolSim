
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"
#include "invsqrt.h"
#include "ParticleContainer.h"

#include <algorithm>
#include <Eigen>
#include <iostream>


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

/**
 * @brief Adds the Force between two Particles to their respective force-parameters
 * 
 * @param p1 
 * @param p2 
 */
void forceBetw2Particles(Particle& p1, Particle& p2);

constexpr double start_time = 0;
double end_time;
double delta_t;

// TODO: what data structure to pick?
ParticleContainer particle_wrapper;
//std::vector<Particle> particles;

int main(int argc, char *argsv[]) {

    std::cout << "Hello from MolSim for PSE!" << std::endl;
    if(argc == 2){
        std::cout << "Set delta_t and end_time to standard values since no explicit values were given " << std::endl;
        delta_t = 0.014;
        end_time = 1000;
    }
    else if (argc != 4) {
        std::cout << "Erroneous programme call! " << std::endl;
        std::cout << "./molsym filename delta_t end_time" << std::endl;
        exit(-1);
    }
    else{
        end_time = atof(argsv[3]);
        delta_t = atof(argsv[2]);
    }

    particle_wrapper = ParticleContainer(argsv[1]);
    //FileReader fileReader;
    //fileReader.readFile(particles, argsv[1]);
    //std::for_each(particles_dummy.begin(), particles_dummy.end(), [&](Particle& p){particles.emplace_back(p);});

    //prepare VTK output
    outputWriter::VTKWriter vtkWriter{};

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
            //plotParticles(iteration);
            vtkWriter.initializeOutput(particle_wrapper.size());
            //vtkWriter.initializeOutput(particles.size());
            //TODO: kill the use of getParticles here (if you find the time)
            for (auto& p : particle_wrapper.getParticles()) vtkWriter.plotParticle(p);
            vtkWriter.writeFile(std::string(argsv[1]) , iteration);
        }
        if(iteration%1000 == 0){
            std::cout << "Iteration " << iteration << " finished." << std::endl;
        }
        
        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}

void calculateF() {
    //set all current forces on all particles to 0
    particle_wrapper.forAllParticles([](Particle& p){
        p.setOldF(p.getF());
        p.setF({0.,0.,0.});
    });

    particle_wrapper.forAllPairs(forceBetw2Particles);
}

void calculateX() {
    particle_wrapper.forAllParticles([](Particle& p){
        Eigen::Vector3d x = delta_t * p.getV() + delta_t * delta_t * p.getOldF() / (2 * p.getM());
        p.add_to_X(x);
    });
}

void calculateV() {
    particle_wrapper.forAllParticles([](Particle& p){
        Eigen::Vector3d v = delta_t * (p.getOldF() + p.getF()) / (2 * p.getM());
        p.add_to_V(v);
    });
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    //TODO: kill the use of getParticles here (if you find the time)
    writer.plotParticles(particle_wrapper.getParticles(), out_name, iteration);
}

void forceBetw2Particles(Particle& p1, Particle& p2){
    double delta_x = p1.getX()[0] - p2.getX()[0];
    double delta_y = p1.getX()[1] - p2.getX()[1];
    double scalar =
            #ifndef INV
            p1.getM() * p2.getM() * std::pow(1/std::sqrt(delta_x * delta_x + delta_y * delta_y), 3);
            #else
            p1.getM() * p2.getM() * std::pow(invsqrtQuake(std::pow(delta_x, 2) + std::pow(delta_y, 2)), 3);
            #endif
    double F_X = -delta_x * scalar;
    double F_Y = -delta_y * scalar;
    p1.add_to_F({F_X, F_Y, 0.});
    p2.add_to_F({-F_X, -F_Y, 0.});
}