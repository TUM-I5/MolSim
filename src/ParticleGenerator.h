#pragma once

#include "Particle.h"
#include "ParticleContainer.h"
#include "Body.h"

#include <Eigen>
#include <list>

namespace ParticleGenerator{
    static int bodyID = 1;

    /**
     * @brief generates  Cuboid with the parameters defined in the Body
     * Body.shape has to be Cuboid
     * Feeds the generated particles into @param buffer
     * @param body is the desired body
     * @param v_bolz is the thermal friction
     */
    static void generateCuboid(struct Body& body, double v_bolz, std::list<Particle>& buffer);

    /**
     * @brief generates Particle at said point
     * 
     * @param x 
     * @param v 
     * @param m
     *
     * and stores it in @param buffer.
     */
    static void generateParticle(std::array<double,3>& x, std::array<double, 3>& v, double m, std::list<Particle>& buffer);

    /**
     * Retrieves an global unique ID for a body.
     * ID 0 is reserved for single particles.
     * */
    static int getNextBodyID();
};
