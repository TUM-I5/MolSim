//
// Created by alex on 30.10.2022.
//

#pragma once

#include "defaults.h"
#include "data/Particle.h"
#include "data/ParticleContainer.h"
#include "io/output/VTKWriter.h"
#include "io/output/Logging.h"
#include "io/IOWrapper.h"
#include "io/input/sim_input/BodyReader.h"
#include "sim/physics/force/FLennardJonesOMP.h"
#include "sim/physics/force/types.h"
#include "sim/physics/position/XStoermerVelvetOMP.h"
#include "sim/physics/velocity/VStoermerVelvetOMP.h"
#include "sim/physics/bounds/types.h"
#include "sim/physics/bounds/BoundsFunctorBase.h"
#include "sim/physics/bounds/BoundsHandler.h"
#include "sim/physics/position/types.h"
#include "sim/physics/velocity/types.h"
#include "sim/physics/thermostat/Thermostat.h"
#include "io/input/Configuration.h"

#include <memory>
#include <chrono>
#include <utility>

namespace sim {
    using namespace sim::physics;
    /**
     * Wrapper for the actually used implementations during the simulation for the different calculation methods.
     * */
    class Simulation {
    private:
        io::IOWrapper &ioWrapper;
        ParticleContainer &particleContainer;
        double start_time;
        double end_time;
        double delta_t;
        /**Globally valid for some functors*/
        double epsilon;
        /**Globally valid for some functos*/
        double sigma;
        const std::string outputFolder;
        const std::string outputBaseName;
        const bool linkedCell;
        const bool checkpointingEnable;
        physics::force::ForceFunctorBase *p_calcF;
        physics::PhysicsFunctorBase *p_calcX;
        physics::PhysicsFunctorBase *p_calcV;

        bool thermoEnable;
        Thermostat thermostat;

    public:
        physics::force::ForceFunctorBase &calcF;
        physics::PhysicsFunctorBase &calcX;
        physics::PhysicsFunctorBase &calcV;
        sim::physics::bounds::BoundsHandler handleBounds;

        /**
         * Standard constructor with all params.
         * */
        explicit Simulation(io::IOWrapper &iow, ParticleContainer &pc, double st = default_start_time, double et = default_end_time,
                            double dt = default_delta_t, double eps = default_epsilon, double sig = default_sigma,
                            std::string of = std::string{default_output_folder},
                            std::string on = std::string{default_output_base_name},
                            bounds::type leftBound = bounds::stot(default_boundary_cond_str),
                            bounds::type rightBound = bounds::stot(default_boundary_cond_str),
                            bounds::type topBound = bounds::stot(default_boundary_cond_str),
                            bounds::type botBound = bounds::stot(default_boundary_cond_str),
                            bounds::type frontBound = bounds::stot(default_boundary_cond_str),
                            bounds::type rearBound = bounds::stot(default_boundary_cond_str),
                            force::type forceType = force::stot(default_force_type),
                            position::type posType = position::stot(default_pos_type),
                            velocity::type velType = velocity::stot(default_vel_type),
                            bool lc = default_linked_cell, bool cpe = default_checkpointing, double gG = default_g_grav,
                            bool thermoEn = default_therm, double thermoDelta_t = default_delta_temp, int thermoNTerm = default_n_term,
                            double thermoTTarget = default_t_target, double ThermoTInit = default_t_init, int dimensions = default_dims) :
                ioWrapper(iow),
                particleContainer(pc),
                start_time(st), end_time(et),
                delta_t(dt), epsilon(eps),
                sigma(sig), outputFolder(std::move(of)),
                outputBaseName(std::move(on)), linkedCell(lc), checkpointingEnable(cpe),
                p_calcF(force::generateForce(forceType, st, et, dt, eps, sig, pc, lc, gG)),
                p_calcX(position::generatePosition(posType, st, et, dt, eps, sig, pc)),
                p_calcV(velocity::generateVelocity(velType, st, et, dt, eps, sig, pc)),
                thermoEnable(thermoEn),
                thermostat(pc, thermoTTarget, thermoNTerm, dimensions, thermoDelta_t, ThermoTInit, thermoEn),
                calcF(*p_calcF),
                calcX(*p_calcX),
                calcV(*p_calcV),
                handleBounds(leftBound, rightBound, topBound, botBound, frontBound, rearBound,
                             calcF, st, et, dt, eps, sig, pc) {
            if (p_calcF == nullptr || p_calcX == nullptr || p_calcV == nullptr) {
                io::output::loggers::general->error("Failed to initialize simulation. Malloc failed.");
                exit(-1);
            }
            io::output::loggers::simulation->trace("Sim constructor full used");
        }

        /**
         * Constructor with no boundary information. Will init simulation, s.t. no linked cell will be used.
         * */
        explicit Simulation(io::IOWrapper &iow, ParticleContainer &pc, double st, double et, double dt, double eps, double sig,
                            const std::string &of, const std::string &on,
                            force::type forceType = force::stot(default_force_type),
                            position::type posType = position::stot(default_pos_type),
                            velocity::type velType = velocity::stot(default_vel_type)) :
                Simulation(iow, pc, st, et, dt, eps, sig, of, on,
                           bounds::type::outflow,
                           bounds::type::outflow,
                           bounds::type::outflow,
                           bounds::type::outflow,
                           bounds::type::outflow,
                           bounds::type::outflow,
                           forceType, posType, velType, false, default_checkpointing) {
            io::output::loggers::simulation->trace("Sim constructor long no lc used");
        }

        /**
         * Constructor that initializes simulation according to configuration object.
         * */
        Simulation(io::IOWrapper &iow, ParticleContainer &pc, io::input::Configuration &config) :
                Simulation(iow, pc, config.get<io::input::startTime>(), config.get<io::input::endTime>(),
                           config.get<io::input::delta_t>(), config.get<io::input::epsilon>(),
                           config.get<io::input::sigma>(),
                           config.get<io::input::outputFilePath>(), config.get<io::input::outputFileName>(),
                           config.get<io::input::boundCondLeft>(), config.get<io::input::boundCondRight>(),
                           config.get<io::input::boundCondTop>(), config.get<io::input::boundCondBottom>(),
                           config.get<io::input::boundCondFront>(), config.get<io::input::boundCondRear>(),
                           config.get<io::input::forceCalculation>(), config.get<io::input::positionCalculation>(),
                           config.get<io::input::velocityCalculation>(),
                           config.get<io::input::linkedCell>(), config.get<io::input::checkpointingEnable>(),
                           config.get<io::input::gGrav>(), config.get<io::input::thermoEnable>(), config.get<io::input::thermoDelta_t>(),
                           config.get<io::input::thermoNTerm>(), config.get<io::input::thermoTTarget>(),
                           config.get<io::input::thermoTInit>(), config.get<io::input::dimensions>()) {
            io::output::loggers::simulation->trace("Sim constructor short used");
        }

        /**
         * Need to clean up due to polymorphism.
         * */
        ~Simulation() {
            delete p_calcF;
            delete p_calcX;
            delete p_calcV;
        }

        /**
         * Runs the simulation loop
         * @param writeParticle Function to write all particles all 10 iterations
         * */
        void run(io::input::Configuration &config) {
            io::output::loggers::simulation->info("Starting simulation");
            double current_time = start_time;
            int iteration = config.get<io::input::simLastIteration>();
            // init forces
            calcF();
            // for this loop, we assume: current x, current f and current v are known
            while (current_time < end_time) {
                calcX();
                calcF();
                if (linkedCell) handleBounds();
                calcV();
                if(thermoEnable){thermostat.notify();}

                iteration++;
                if (iteration % 10 == 0) {
                    if (linkedCell) particleContainer.updateCells(); // update cell structure
                    ioWrapper.writeParticlesVTK(particleContainer, outputFolder, outputBaseName, iteration);
                }
                if (iteration % 1000 == 0) {
                    if(checkpointingEnable) ioWrapper.writeCheckpoint(particleContainer, config, iteration, current_time);
                    io::output::loggers::simulation->info("Progress: {:03.2f}%", current_time / end_time * 100);
                    io::output::loggers::simulation->trace("Iteration {} finished.", iteration);
                }

                current_time += delta_t;
            }
            //write final checkpoint to allow further calculation
            if(checkpointingEnable) ioWrapper.writeCheckpoint(particleContainer, config, iteration, current_time);
        }

        /**
         * Runs the simulation and measures elapsed time. No output will be written during execution.
         * @param simIteration Amount of iterations the simulation should be performed, until average time is computed.
         * @param inputDataSource Name of where the starting data came from
         * @param startingData Data based on which the simulation should start
         * */
        void runBenchmark(const int simIteration, const std::string &inputDataSource,
                          const std::vector<Particle> &startingData, double bbox0, double bbox1, double bbox2, double rCutoff) {
#pragma region sim_times
            //get times for total simulation
            std::chrono::high_resolution_clock::duration sim_duration{std::chrono::high_resolution_clock::duration::zero()};
            std::chrono::high_resolution_clock::duration sim_minTime{std::chrono::high_resolution_clock::duration::max()};
            std::chrono::high_resolution_clock::duration sim_maxTime{std::chrono::high_resolution_clock::duration::min()};

            for (int pass{0}; pass < simIteration; pass++) {
                //reset data
                ParticleContainer pc = ParticleContainer(startingData,{bbox0, bbox1, bbox2},rCutoff);
                particleContainer = pc;
                calcF.setParticleContainer(particleContainer);
                calcX.setParticleContainer(particleContainer);
                calcV.setParticleContainer(particleContainer);
                handleBounds.setParticleContainer(particleContainer);
                thermostat.setParticleContainer(pc);

                //get time stamp
                auto startTime = std::chrono::high_resolution_clock::now();

                //======================================
                double current_time = start_time;
                int iteration = 0;
                calcF();
                while (current_time < end_time) {
                    calcX();
                    calcF();
                    if (linkedCell) handleBounds();
                    calcV();
                    if(thermoEnable){thermostat.notify();}
                    if (iteration % 10 == 0) {
                        if (linkedCell) particleContainer.updateCells();
                    }
                    current_time += delta_t;
                    iteration++;
                }
                //======================================

                //get timings
                auto endTime = std::chrono::high_resolution_clock::now();
                auto delta = endTime - startTime;
                if (delta < sim_minTime) sim_minTime = delta;
                if (delta > sim_maxTime) sim_maxTime = delta;
                sim_duration += delta;

                io::output::loggers::simulation->debug("Progress: SIM-TIME: {:03.2f}%", static_cast<double>(pass) / static_cast<double>(simIteration) * 100);
                io::output::loggers::simulation->trace("Finished pass {}", pass);
            }

            auto sim_durationMillis =std::chrono::duration_cast<std::chrono::milliseconds>(sim_duration).count() / simIteration;
            auto sim_minMillis = std::chrono::duration_cast<std::chrono::milliseconds>(sim_minTime).count();
            auto sim_maxMillis = std::chrono::duration_cast<std::chrono::milliseconds>(sim_maxTime).count();
#pragma endregion

#pragma region iteration_times
            //get times for total simulation
            std::chrono::high_resolution_clock::duration it_duration{std::chrono::high_resolution_clock::duration::zero()};
            std::chrono::high_resolution_clock::duration it_minTime{std::chrono::high_resolution_clock::duration::max()};
            std::chrono::high_resolution_clock::duration it_maxTime{std::chrono::high_resolution_clock::duration::min()};

            //reset data
            ParticleContainer pc = ParticleContainer(startingData,{bbox0, bbox1, bbox2},rCutoff);
            particleContainer = pc;
            calcF.setParticleContainer(particleContainer);
            calcX.setParticleContainer(particleContainer);
            calcV.setParticleContainer(particleContainer);
            handleBounds.setParticleContainer(particleContainer);
            thermostat.setParticleContainer(pc);

            //init forces
            calcF();
            //perform measurement
            for (int iteration{0}; iteration < simIteration; iteration++) {
                //get time stamp
                auto startTime = std::chrono::high_resolution_clock::now();

                //======================================
                calcX();
                calcF();
                if (linkedCell) handleBounds();
                calcV();
                if(thermoEnable){thermostat.notify();}
                if (iteration % 10 == 0) {
                    if (linkedCell) particleContainer.updateCells();
                }
                //======================================

                //get timings
                auto endTime = std::chrono::high_resolution_clock::now();
                auto delta = endTime - startTime;
                if (delta < it_minTime) it_minTime = delta;
                if (delta > it_maxTime) it_maxTime = delta;
                it_duration += delta;

                io::output::loggers::simulation->debug("Progress: IT-TIME: {:03.2f}%", static_cast<double>(iteration) / static_cast<double>(simIteration) * 100);
                io::output::loggers::simulation->trace("Finished iteration {}", iteration);
            }

        auto it_durationNanos =
                std::chrono::duration_cast<std::chrono::nanoseconds>(it_duration).count() / simIteration;
        auto it_minNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(it_minTime).count();
        auto it_maxNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(it_maxTime).count();
#pragma endregion
        io::output::loggers::simulation->info(
                "##SimulationData:{}|Passes:{}|Particles:{}## [SIM-TIME:MS] AVG:{}|MIN:{}|MAX:{} [IT-TIME:NS] AVG:{}|MIN:{}|MAX:{}", inputDataSource,
                simIteration, startingData.size(), sim_durationMillis, sim_minMillis, sim_maxMillis, it_durationNanos, it_minNanos, it_maxNanos);
    }
};
} // sim