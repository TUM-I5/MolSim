//
// Created by alex on 30.10.2022.
//

#pragma once

#include "data/Particle.h"
#include "data/ParticleContainer.h"
#include "io/outputWriter/VTKWriter.h"
#include "io/Logging.h"

namespace sim {
    extern ParticleContainer particleContainer;
    extern double start_time;
    extern double end_time;
    extern double delta_t;
    extern double epsilon;
    extern double sigma;
    extern std::string outputFolder;
    extern std::string outputBaseName;

    constexpr double default_delta_t{0.014};
    constexpr double default_end_time{1000};
    constexpr double default_start_time{0};
    constexpr auto default_output_base_name{"result"};
    constexpr auto default_output_folder{"./output/"};
    constexpr double default_epsilon{1};
    constexpr double default_sigma{1};

    /**
    * calculate the force for all particles by gravitation.
    */
    void calculateFGravity();

    /**
     * calculate the force for all particles using the Lennard-Jones potential
     * */
    void calculateFLennardJones();

    /**
    * calculate the position for all particles using the Stoermer Velvet method.
    */
    void calculateXStoermerVelvet();

    /**
    * calculate the position for all particles using the Stoermer Velvet method.
    */
    void calculateVStoermerVelvet();

    /**
     * Wrapper for the actually used implementations during the simulation for the different calculation methods.
     * */
    template<void calcF() = calculateFGravity, void calcX() = calculateXStoermerVelvet, void calcV() = calculateVStoermerVelvet>
    class Simulation {
    public:
        Simulation(double st = default_start_time, double et = default_end_time, double dt = default_delta_t,
                   double eps = default_epsilon, double sig = default_sigma,
                   const std::string &of = std::string{default_output_folder},
                   const std::string &on = std::string{default_output_base_name}) {
            sim::start_time = st;
            sim::end_time = et;
            sim::delta_t = dt;
            sim::epsilon = eps;
            sim::sigma = sig;
            sim::outputFolder = of;
            sim::outputBaseName = on;
        }

        /**
         * Runs the simulation loop
         * */
        void run() {
            //prepare VTK output
            outputWriter::VTKWriter vtkWriter{};
            double current_time = sim::start_time;
            int iteration = 0;
            // init forces
            calcF();
            // for this loop, we assume: current x, current f and current v are known
            while (current_time < sim::end_time) {
                calcX();
                calcF();
                calcV();

                iteration++;
                if (iteration % 10 == 0) {
                    vtkWriter.initializeOutput(sim::particleContainer.size());
                    for (auto &p: sim::particleContainer) vtkWriter.plotParticle(p);
                    vtkWriter.writeFile(outputFolder + outputBaseName, iteration);
                }
                if (iteration % 1000 == 0) {
                    loggers::simulation->debug("Iteration {} finished.", iteration);
                }

                current_time += sim::delta_t;
            }
        }
    };
} // sim