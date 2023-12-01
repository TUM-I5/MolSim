
#pragma once

#include "outputWriter/VTKWriter.h"
#include "Particle.h"


class SimulationContainer{
    public:
    /**
     * Functions acting on ParticleContainer
     */

    virtual void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                    double m_arg) = 0;

    virtual void reserve(size_t) = 0;                 

    virtual void plotParticles(outputWriter::VTKWriter &writer) = 0;

    virtual std::string to_string() = 0;

    virtual size_t size() const = 0;


};