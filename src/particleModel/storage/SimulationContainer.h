
#pragma once

#include "outputWriter/VTKWriter.h"
#include "Particle.h"

/**
 * @class SimulationContainer
 * @brief abstract class representing a Container that encapsulates Particles
 *        offers four abstract methods
 * 
 * (mainly used in runSimulation and addCuboids/ addSpheres)
 */
class SimulationContainer{
    public:

    /**
     * @brief creates and adds particle with provided parameters
     *
     * @param x_arg position of the particle
     * @param v_arg velocity of the particle
     * @param m_arg mass of the particle
     */
    virtual void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                    double m_arg) = 0;
          
    /**
     * @brief plots all particles into a VTK output
     *
     * @param writer to provide plotting method
     */
    virtual void plotParticles(outputWriter::VTKWriter &writer) = 0;


    
    virtual std::string to_string() = 0;
    

    virtual size_t size() = 0;


};