#pragma once

#include "Particle.h"
#include "ParticleContainer.h"
#include "Body.h"

#include <Eigen>
#include <list>

namespace ParticleGenerator{
    extern int bodyID;

    /**
     * @brief generates  Cuboid with the parameters defined in the Body
     * Body.shape has to be Cuboid
     * Feeds the generated particles into @param buffer
     * @param body is the desired body
     * @param v_bolz is the thermal friction
     * @param dims is the dimension of the data
     */
    void generateCuboid(struct Body& body, double v_bolz, std::list<Particle>& buffer, int dims);

    /**
     * @brief generates Sphere at said point
     * Body.shape has to be sphere
     * Feeds the generated particles int @param buffer
     * @param body is the desired body 
     * @param v_bolz is the thermal friction
     * @param dims is the dimension of the data
     */
    void generateSphere(struct Body& body, double v_bolz, std::list<Particle>& buffer, int dims);

    /**
     * @brief generates Particle at said point
     * 
     * @param x 
     * @param v 
     * @param m
     *
     * and stores it in @param buffer.
     */
    void generateParticle(Eigen::Vector3d& x, Eigen::Vector3d& v, double m, std::list<Particle>& buffer);

    /**
     * Retrieves an global unique ID for a body.
     * ID 0 is reserved for single particles.
     * */
    int getNextBodyID();
};
