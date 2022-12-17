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
#include "LennardJonesForce.h"
#include "InterParticleGravitationalForce.h"
#include "SingleParticleGravitationalForce.h"
#include "../model/ProgramParameters.h"

#include <iostream>

Simulation::Simulation(ProgramParameters *programParameters)
{
    _programParameters = programParameters;
    _interParticleForceCalculation.reset(new LennardJonesForce());
    _singleParticleForceCalculation.reset(new SingleParticleGravitationalForce());
    _logicLogger = spdlog::get("simulation_logger");
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("Simulation generated!");
}

Simulation::~Simulation()
{
    _memoryLogger->info("Simulation destructed!");
}

const void Simulation::simulate()
{
    double start_time = 0;
    double current_time = start_time;

    int iteration = 0;

    OutputFacade outputFacade = OutputFacade(_programParameters->getParticleContainer(), _programParameters->getBaseName());
    outputFacade.outputVTK(iteration);

    // calculating force once to initialize force
    _memoryLogger->info("Initial force calculation");
    _interParticleForceCalculation->calculateForce(*_programParameters->getParticleContainer());
    _singleParticleForceCalculation->calculateForce(*_programParameters->getParticleContainer(), _programParameters->getGGrav()); 

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < _programParameters->getEndTime())
    {
        // calculate new x
        calculateX();
        // calculate new f
        _interParticleForceCalculation->calculateForce(*_programParameters->getParticleContainer());
        _singleParticleForceCalculation->calculateForce(*_programParameters->getParticleContainer(), _programParameters->getGGrav()); 
        // calculate new v
        calculateV();

        iteration++;
        if (iteration % _programParameters->getWriteFrequency() == 0 && _programParameters->getBenchmarkIterations() == 0)
        {
            outputFacade.outputVTK(iteration);
        }
        _logicLogger->info("Iteration {} finished.", iteration);

        current_time += _programParameters->getDeltaT();
    }

    _logicLogger->info("Finished Iterations. Terminating");
}

void Simulation::calculateX()
{
    std::shared_ptr<ParticleContainer> particleContainer = _programParameters->getParticleContainer();

    // creating lambda to calculate new position based on the Velocity-Störmer-Verlet algortihm
    std::function<void(Particle &)> f = [delta_t = _programParameters->getDeltaT()](Particle &p1)
    {
        std::array<double, 3> x_new = p1.getX() + delta_t * p1.getV() + (delta_t * delta_t / (2 * p1.getM())) * p1.getF();
        p1.setX(x_new);
    };

    particleContainer->iterateParticles(f);
}

void Simulation::calculateV()
{
    std::shared_ptr<ParticleContainer> particleContainer = _programParameters->getParticleContainer();

    // creating lambda to calculate new speed based on the Velocity-Störmer-Verlet algortihm
    std::function<void(Particle &)> f = [delta_t = _programParameters->getDeltaT()](Particle &p1)
    {
        std::array<double, 3> v_new = p1.getV() + (delta_t / (2 * p1.getM())) * (p1.getOldF() + p1.getF());
        p1.setV(v_new);
    };

    particleContainer->iterateParticles(f);
}

const std::shared_ptr<spdlog::logger> Simulation::getLogicLogger() const { return _logicLogger; }
