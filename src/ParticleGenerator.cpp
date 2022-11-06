#include "ParticleGenerator.h"
#include "utils/MaxwellBoltzmannDistribution.h"

#include <iostream>
#include <memory>

ParticleGenerator::ParticleGenerator(ParticleContainer& container){
    particleContainer = container;
}

void ParticleGenerator::setParticleContainer(ParticleContainer& container){
    particleContainer = container;
}

void ParticleGenerator::generateCuboid(struct Body& body, double v_bolz){ 
    //Maybe it would be more efficient to concatenate two vectors instead of placing one particle after another in the ParticleContainer
    if(body.shape != cuboid){
        std::cerr<<"generateCuboid does not work for shapes that aren't Cuboids";
    }
    for (size_t x = 0; x < body.dimensions[0]; x++)
    {
        for (size_t y = 0; y < body.dimensions[1]; y++)
        {
            for (size_t z = 0; z < body.dimensions[2]; z++)
            {
                Eigen::Vector3d pos = body.fixpoint + (body.distance * Eigen::Vector3d(x,y,z));
                Eigen::Vector3d v = Eigen::Vector3d(maxwellBoltzmannDistributedVelocity(v_bolz, 3)); 
                auto p = Particle{pos, body.start_velocity +  v, body.mass};
                particleContainer.addParticle(p);
            }
        }
    }
}