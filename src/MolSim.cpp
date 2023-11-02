
#include "FileReader.h"
#include "outputWriter/XYZWriter.h"
#include "outputWriter/VTKWriter.h"
#include "utils/ArrayUtils.h"
#include "ParticleContainer.h"

#include <iostream>
#include <list>
#include <string>
#include <unistd.h>

/**** forward declaration of the calculation functions ****/

/**
 *
 * @brief calculates the force of every Particle at the current time step
 *
 * for every particle all the forces acting on that particle are summed up, while
 * iterating over all unique pairs of particles {p_i,p_j}, where p_i != p_j. Meaning
 * for the pair {p_i,p_j}, the force f_i_j is calulated and added / substracted
 * to F_i / F_j (on which added/substracted depends on order,in which iterated over the particles)
 * After that the next f_i_j for the next unique pair is calculated
 *
 * @param None
 * @return None
 *
 */
void calculateF();

/**
 * 
 * @brief calculates the position of every particle for the next timestep according to given formula
 *          
 * 
 * @param None
 * @return None
 */
void calculateX();

/**
 * @brief calculates the velocity of every particle for the next timestep according to given formula
 * 
 * @param None
 * @return None
 */
void calculateV();

/**
 * @brief plot the particles to a xyz-file
 * 
 * @param None
 * @return None
 */
void plotParticles(int iteration);


/**
 *
 * @brief for every particle change old force attribute to the current force
 *
 * called after every iteration/ when new X,V and F was calculated
 * sets  the Particle.old_f (old force) for the next time step to be the Particle.f (current force) of this
 * time step
 *
 * @param None
 * @return None
 *
 */
void shiftForces();

constexpr double start_time = 0;
double end_time = 1500;
double delta_t = 0.014;
char* filename;

ParticleContainer particleContainer;

int main(int argc, char *argsv[]) {

    auto msg = "Usage ./MolSim [-e<double>] [-t<double>] -f<String>\n"
               " -e<double>:      gives the end_time of the simulation\n"
               " -t<double>:      gives the step size used for the simulation\n"
               " -f<String>:      gives the filename from which the initial state\n"
               "                  of the Particles is read, these are the particles\n"
               "                  that will get simulated\n"
               "\n"
               "Returns:\n"
               "                  several .vtu files that can be used for visualisation in Paraview\n";

    std::cout << "Hello from MolSim for PSE!" << std::endl;
    int opt;
    while ((opt = getopt(argc, argsv, "t:e:f:h")) != -1) {
        switch (opt) {
            case 't':
                try {
                    delta_t = std::stod(optarg);
                    std::cout << "delta_t: " << delta_t << std::endl;
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument for delta_t" << e.what() << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "The delta_t is Out of range" << e.what() << std::endl;
                }
                break;
            case 'e':
                try {
                    end_time = std::stod(optarg);
                    std::cout << "end_time: " << end_time << std::endl;
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument for the endtime" << e.what() << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "The endtime is Out of Range" << e.what() << std::endl;
                }

                break;
            case 'f':
                filename = optarg;
                break;
            case 'h':
                std::cout << msg;
                return 0;
            default:
                std::cerr << msg;
                return 1;
        }
    }

    outputWriter::VTKWriter writer;
    FileReader fileReader;
    fileReader.readFile(particleContainer, filename);
    writer.initializeOutput(particleContainer.size());

    double current_time = start_time;

    int iteration = 0;

    //calculate inital force:
    std::cout << "calculate initial force" << std::endl;
    calculateF();
    shiftForces();
    particleContainer.printParticles();
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
            writer.initializeOutput(particleContainer.size());
            particleContainer.plotParticles(writer);
            writer.writeFile("out", iteration);

        }
        //std::cout << "calc shift Forces" << std::endl;
        shiftForces();
        std::cout << "Iteration " << iteration << " finished." << std::endl;

        current_time += delta_t;
    }

    std::cout << "output written. Terminating..." << std::endl;
    return 0;
}



void calculateF() {
    static std::pair<Particle*, Particle*> pair = std::make_pair(nullptr, nullptr);
    particleContainer.setNextPair(pair);

    while(pair.first != nullptr){
        const double &m_i = pair.first->getM();
        const double &m_j = pair.second->getM();
        const std::array<double, 3> &x_i = pair.first->getX();
        const std::array<double, 3> &x_j = pair.second->getX();
        double c = (m_i * m_j) / std::pow(ArrayUtils::L2Norm(x_i - x_j), 3);

        for(int k = 0; k < 3; k++) {
            double F_ij_k = c * (x_j[k] - x_i[k]);
            pair.first->addF(k, F_ij_k);
            pair.second->addF(k, -F_ij_k);
        }

        particleContainer.setNextPair(pair);
    }
}

void calculateX() {
    Particle* p = particleContainer.getNextParticle();

    while(p != nullptr) {
        const std::array<double, 3> &old_x = p->getX();
        const std::array<double, 3> &v = p->getV();
        const std::array<double, 3> &f = p->getOldF();
        const double &m = p->getM();

        for (int i = 0; i < 3; i++) {
            p->setX(i, old_x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);
        }

        p = particleContainer.getNextParticle();
    }
}

void calculateV() {
    Particle* p = particleContainer.getNextParticle();

    while(p != nullptr) {
        const std::array<double, 3> &v = p->getV();
        const std::array<double, 3> &f = p->getF();
        const std::array<double, 3> &f_old = p->getOldF();
        const double &m = p->getM();

        for (int i = 0; i < 3; i++) {
            p->setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
        }

        p = particleContainer.getNextParticle();
    }
}

void shiftForces() {
    Particle* p = particleContainer.getNextParticle();
    while(p != nullptr) {
        p->shiftF();
        p = particleContainer.getNextParticle();
    }
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticles(particleContainer, out_name, iteration);
}


