//
// Created by alex on 30.10.2022.
//

#pragma once

#include "data/Particle.h"
#include "data/ParticleContainer.h"
#include "io/outputWriter/VTKWriter.h"
#include "io/Logging.h"
#include "io/IOWrapper.h"
#include "io/BodyReader.h"

#include <memory>
#include <chrono>

namespace sim {
    extern ParticleContainer particleContainer;
    extern double start_time;
    extern double end_time;
    extern double delta_t;
    extern double epsilon;
    extern double sigma;
    extern std::string outputFolder;
    extern std::string outputBaseName;


    /**
    * calculate the force for all particles by gravitation.
    */
    void calculateFGravity();

    /**
     * calculate the force for all particles using the Lennard-Jones potential
     * */
    void calculateFLennardJones();

    /**
     * calculate the force for all particles using the Lennard-Jones potential, fast
     * */
    void calculateFLennardJonesFast();

    /**
    * calculate the position for all particles using the Stoermer Velvet method. fast
    */
    void calculateXStoermerVelvetFast();

    /**
    * calculate the position for all particles using the Stoermer Velvet method. fast
    */
    void calculateVStoermerVelvetFast();

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
            loggers::simulation->info("Starting simulation");
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
                    io::ioWrapper->writeParticlesVTK(sim::particleContainer, outputFolder, outputBaseName, iteration);
                }
                if (iteration % 1000 == 0) {
                    loggers::simulation->info("Progress: {:03.2f}%", current_time/sim::end_time*100);
                    loggers::simulation->debug("Iteration {} finished.", iteration);
                }

                current_time += sim::delta_t;
            }
        }

        /**
         * Runs the simulation and measures elapsed time. No output will be written during execution.
         * @param simIteration Amount of iterations the simulation should be performed, until average time is computed.
         * @param inputDataSource Name of where the starting data came from
         * @param startingData Data based on which the simulation should start
         * */
        void runBenchmark(const int simIteration, const std::string &inputDataSource,
                          const std::vector<Particle> &startingData) {
            std::chrono::high_resolution_clock::duration duration{std::chrono::high_resolution_clock::duration::zero()};
            std::chrono::high_resolution_clock::duration minTime{std::chrono::high_resolution_clock::duration::zero()};
            std::chrono::high_resolution_clock::duration maxTime{std::chrono::high_resolution_clock::duration::zero()};

            for (int pass{0}; pass < simIteration; pass++) {
                //reset data
                sim::particleContainer = ParticleContainer(startingData);

                //get time stamp
                auto startTime = std::chrono::high_resolution_clock::now();

                //======================================
                double current_time = sim::start_time;
                calcF();
                while (current_time < sim::end_time) {
                    calcX();
                    calcF();
                    calcV();
                    current_time += sim::delta_t;
                }
                //======================================

                //get timings
                auto endTime = std::chrono::high_resolution_clock::now();
                auto delta = endTime - startTime;
                if (delta < minTime) minTime = delta;
                if (delta > maxTime) maxTime = delta;
                duration += delta;

                loggers::simulation->debug("Finished pass {}", pass);
            }

            auto durationMillis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / simIteration;
            auto minMillis = std::chrono::duration_cast<std::chrono::milliseconds>(minTime).count();
            auto maxMillis = std::chrono::duration_cast<std::chrono::milliseconds>(maxTime).count();

            loggers::simulation->info("###SimulationData:{}|Iterations:{}|AVG:{}|MIN:{}|MAX:{}|Particles:{}", inputDataSource,
                                      simIteration, durationMillis, minMillis, maxMillis, startingData.size());
        }
    };
} // sim