#pragma once

#include "Particle.h"
#include "ParticleContainer.h"
#include "Body.h"

#include <Eigen>

class ParticleGenerator{

private:
/**
 * @brief The ParticleContainer that will be fed all the generated Particles
 * 
 */
ParticleContainer particleContainer;

public:

    /**
     * @brief ParticleContainer already known
     * 
     */
    ParticleGenerator(ParticleContainer&);

    /**
     * @brief ParticleContainer not known yet
     * 
     */
    ParticleGenerator();

    /**
     * @brief Sets the ParticleContainer to given value
     * 
     */
    void setParticleContainer(ParticleContainer&);

    /**
     * @brief generates  Cuboid with the parameters defined in the Body
     * Body.shape has to be Cuboid
     * Feeds the generated particles into the ParticleContainer
     */
    void generateCuboid(struct Body body, double v_bolz);

    /**
     * @brief generates Particle at said point
     * 
     * @param x 
     * @param v 
     * @param m 
     */
    void generateParticle(std::array<double,3>& x, std::array<double, 3>& v, double m);
};
