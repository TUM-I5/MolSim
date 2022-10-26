
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "utils/ArrayUtils.h"
#include "invsqrt.h"

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

constexpr double start_time = 0;
constexpr double end_time = 1000;
constexpr double delta_t = 0.014;

// TODO: what data structure to pick?
//std::list<Particle> particles_dummy;
std::vector<Particle> particles;

int main(int argc, char *argsv[]) {

    std::cout << "Hello from MolSim for PSE!" << std::endl;
    if (argc != 2) {
        std::cout << "Erroneous programme call! " << std::endl;
        std::cout << "./molsym filename" << std::endl;
        exit(-1);
    }

    FileReader fileReader;
    fileReader.readFile(particles, argsv[1]);
    //std::for_each(particles_dummy.begin(), particles_dummy.end(), [&](Particle& p){particles.emplace_back(p);});

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
    //set all current forces on all particles to 0
    std::for_each(particles.begin(), particles.end(), [&](Particle &p) {
        p.setOldF(p.getF());
        p.setF({0., 0., 0.});
    });
    //for every particle:
    for (u_int32_t i = 0; i < particles.size(); i++) {
        //for every particle with higher index:
        for (u_int32_t j = i + 1; j < particles.size(); j++) {
            //compute force on pi from pj; pi_sum += computed_force
            // pj_sum = -computed_force
            Particle &p1 = particles[i];
            Particle &p2 = particles[j];
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
    }
}

void calculateX() {
    for (auto &p: particles) {
        // @TODO: insert calculation of force here!
        Eigen::Vector3d x = delta_t * p.getV() + delta_t * delta_t * p.getOldF() / (2 * p.getM());
        p.add_to_X(x);
        //double x_0 = delta_t * p.getV()[0] + delta_t * delta_t * p.getOldF()[0] / (2*p.getM());
        //double x_1 = delta_t * p.getV()[1] + delta_t * delta_t * p.getOldF()[1] / (2*p.getM());
        //p.add_to_X({x_0, x_1, 0.});
    }
}

void calculateV() {
    for (auto &p: particles) {
        // @TODO: insert calculation of force here!
        Eigen::Vector3d v = delta_t * (p.getOldF() + p.getF()) / (2 * p.getM());
        p.add_to_V(v);
        //double v_0 = delta_t * (p.getOldF()[0] + p.getF()[0]) / (2 * p.getM());     //p.getV()(0) to get x-coordinate of Vector
        //double v_1 = delta_t * (p.getOldF()[1] + p.getF()[1]) / (2 * p.getM());
        //p.add_to_V({v_0, v_1, 0.});
    }
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticles(particles, out_name, iteration);
}
