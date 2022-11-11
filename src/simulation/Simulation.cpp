/*
 * Simulation.cpp
 * 
 * Created: 31.10.2022 
 * Author:  wohlrapp 
*/

#include "Simulation.h"
#include "../outputWriter/VTKWriter.h"
#include "../utils/ArrayUtils.h"
#include "../outputWriter/OutputFacade.h"

#include <iostream>


Simulation::Simulation(ParticleContainer &particleContainer_arg, double end_time_arg, double delta_t_arg){
    particleContainer = particleContainer_arg;
    end_time = end_time_arg; 
    delta_t = delta_t_arg;

    _logicLogger = spdlog::get("simulation_logger");
}

const void Simulation::simulate(){
    double start_time = 0; 
    double current_time = start_time;

    int iteration = 0;

    OutputFacade outputFacade = OutputFacade(&particleContainer);

    // calculating force once to initialize force
    calculateF(); 

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < getEndTime()) {
        // calculate new x
        calculateX();
        // calculate new f
        calculateF();
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % 10 == 0) { 
            outputFacade.outputVTK(iteration); 
        }
        _logicLogger->info("Iteration {} finished.", iteration);

        current_time += getDeltaT();
    }

    _logicLogger->info("Finished Iterations. Terminating");
}

void Simulation::calculateX() {
    ParticleContainer &particleContainer = getParticleContainer(); 

    // creating lambda to calculate new position based on the Velocity-Störmer-Verlet algortihm
    std::function<void (Particle &)> f = [delta_t = getDeltaT()] (Particle &p1) {
        std::array<double,3> x_new = p1.getX() + delta_t * p1.getV() + (delta_t * delta_t / (2 * p1.getM()))* p1.getF();
        p1.setX(x_new);
    };

    particleContainer.iterateParticles(f); 
}

void Simulation::calculateV() {
    ParticleContainer &particleContainer = getParticleContainer(); 

    // creating lambda to calculate new speed based on the Velocity-Störmer-Verlet algortihm
    std::function<void (Particle &)> f = [delta_t = getDeltaT()] (Particle &p1) {
        std::array<double, 3> v_new = p1.getV() + (delta_t/ (2*p1.getM())) * (p1.getOldF() + p1.getF());
        p1.setV(v_new);
    };

    particleContainer.iterateParticles(f); 
}

ParticleContainer &Simulation::getParticleContainer() { return particleContainer; }

const double &Simulation::getEndTime() const { return end_time; } 

const double &Simulation::getDeltaT() const { return delta_t; } 

const std::shared_ptr<spdlog::logger> Simulation::getLogicLogger() const { return _logicLogger; }
