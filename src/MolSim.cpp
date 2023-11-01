
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"

#include <iostream>
#include <list>
#include <outputWriter/VTKWriter.h>

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

    //passing arguments via the command line
    double end_time = std::atof(argsv[2]);
    double delta_t = std::atof(argsv[3]);

    double current_time = start_time;

    int iteration = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < end_time) {
        // calculate new x
        calculateX(delta_t);
        // calculate new f
        calculateF();
        // calculate new v
        calculateV(delta_t);

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

template <typename T, std::size_t N>
double secondNorm(const std::array<T, N>& arr) {
    double norm = 0.0;
    for (const T& val : arr) {
        norm += val * val;
    }
    return std::sqrt(norm);
}

void calculateF() {
    std::list<Particle>::iterator iterator;
    iterator = particles.begin();

    for (auto &p1: particles) {
        std::array<double, 3> F_i{0.,0.,0.};
        for (auto &p2: particles) {
            // @TODO: insert calculation of forces here!
            // formula: Fij = ((mi * mj) / ||xi −xj||^3) * (xj − xi)
            std::array<double, 3> F_ij{0.,0.,0.};
            if(&p1 != & p2){
                auto force = p1.getM() * p2.getM() * (p2.getX() - p1.getX());

                for (int i = 0; i < 3; ++i) {
                    F_ij[i] += force[i] / pow(secondNorm((p1.getX() - p2.getX())), 3.0);
                    F_i[i] += F_ij[i];
                }
            }
        }
        p1.setOldF(p1.getF());
        p1.setF(F_i);
    }
}

void calculateX(double delta_t) {
    for (auto &p: particles) {
        // @TODO: insert calculation of position updates here!
        // Calculate xi(tn+1)
        // formula: xi(tn+1) = xi(tn) + ∆t · vi(tn) + (∆t)^2 * (Fi(tn)/2mi)

        auto xi_tn1 = p.getX() + delta_t * p.getV() + (delta_t * delta_t) / (2.0 * p.getM()) * p.getF();

        p.setX(xi_tn1);  // Update the position
    }
}

void calculateV(double delta_t) {
    for (auto &p: particles) {
        // @TODO: insert calculation of velocity updates here!
        // formula: vi(tn+1) = vi(tn) + ∆t * ((Fi(tn) + Fi(tn+1))/ 2mi)
        // Calculate the velocity at time tn+1

        auto vi_tn1 = p.getV() + delta_t / (2 * p.getM()) * (p.getOldF() + p.getF());
        p.setV(vi_tn1);
    }
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::VTKWriter writer;
    writer.initializeOutput(particles.size());
    for(auto &p:particles){
        writer.plotParticle(p);
    }
    writer.writeFile(out_name,iteration);
}
