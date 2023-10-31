
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
 * first for every two particles, the force between them is calculated, then
 * all forces acting on a certain particle are summed up to get the total
 * force acting on that particle in this certain time step
 *
 * @param None
 * @return None
 *
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


ParticleContainer particles;
outputWriter::VTKWriter writer;

int main(int argc, char *argsv[]) {

    auto msg = "Usage ./MolSim -e<double> -t<double> -f<String>\n"
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
                break;
            default:
                std::cerr << msg;
                return 1;
        }
    }



    std::list <Particle> particles_list;
    FileReader fileReader;
    fileReader.readFile(particles_list, filename);
    particles = ParticleContainer(particles_list);
    writer.initializeOutput(particles.size());

    double current_time = start_time;

    int iteration = 0;

    exit(1);
    //calculate inital force:
    std::cout << "calculate initial force" << std::endl;
    calculateF();
    shiftForces();

    for (auto &p: particles) {
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
            for (auto &p: particles) {
                writer.plotParticle(p);
            }
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
    const auto amt_particles = particles.size();
    using Matrix = std::vector <std::vector<std::array < double, 3>>>;
    Matrix forces(amt_particles, std::vector < std::array < double, 3 >> (amt_particles));


    for (size_t i = 0; i < amt_particles; i++) {
        for (size_t j = 0; j < amt_particles; j++) {
            if (i != j) {
                double m_i = particles[i].getM();
                double m_j = particles[j].getM();
                std::array<double, 3> x_i = particles[i].getX();
                std::array<double, 3> x_j = particles[j].getX();
                auto diff = x_j - x_i;
                double norm = ArrayUtils::L2Norm(diff);
                forces[i][j] = ((m_i * m_j) / std::pow(norm, 3)) * diff;
            }
        }
    }
    for (size_t i = 0; i < amt_particles; i++) {
        std::array<double, 3> F_i{0.0, 0.0, 0.0};
        for (size_t j = 0; j < amt_particles; j++) {
            if (i != j)
                F_i = F_i + forces[i][j];
        }
        particles[i].setF(0, F_i[0]);
        particles[i].setF(1, F_i[1]);
        particles[i].setF(2, F_i[2]);
    }
}

void calculateX() {
    for (auto &p: particles) {
        const std::array<double, 3> &old_x = p.getX();
        const std::array<double, 3> &v = p.getV();
        const std::array<double, 3> &f = p.getOldF();
        const double &m = p.getM();
        for (int i = 0; i < 3; i++) {
            p.setX(i, old_x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);
        }
    }
}

void calculateV() {
    for (auto &p: particles) {

        const std::array<double, 3> &v = p.getV();
        const std::array<double, 3> &f = p.getF();
        const std::array<double, 3> &f_old = p.getOldF();
        const double &m = p.getM();

        for (int i = 0; i < v.size(); i++) {
            p.setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
        }
    }
}



void shiftForces() {
    for (auto &p: particles) {
        auto old_F = p.getF();
        p.setOldF(0, old_F[0]);
        p.setOldF(1, old_F[1]);
        p.setOldF(2, old_F[2]);
    }
}

void plotParticles(int iteration) {

    std::string out_name("MD_vtk");

    outputWriter::XYZWriter writer;
    writer.plotParticles(particles, out_name, iteration);
}


