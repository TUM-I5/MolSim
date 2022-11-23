#include "Particle.h"
#include "ParticleGenerator.h"
#include "utils/MaxwellBoltzmannDistribution.h"

#include <iostream>

namespace ParticleGenerator {
    int bodyID = 1;

	void generateCuboid(struct Body& body, double v_bolz, std::list<Particle>& buffer, int dims){ //thermal friction hardcoded to 0.1, is that what we want to do?
        //Maybe it would be more efficient to concatenate two vectors instead of placing one particle after another in the ParticleContainer
        if(body.shape != cuboid){
            std::cerr<<"generateCuboid does not work for shapes that aren't Cuboids";
        }
        int typeID = getNextBodyID();
        for (size_t x = 0; x < body.dimensions[0]; x++)
        {
            for (size_t y = 0; y < body.dimensions[1]; y++)
            {
                for (size_t z = 0; z < body.dimensions[2]; z++)
                {
                    Eigen::Vector3d pos = body.fixpoint + (body.distance * Eigen::Vector3d(x,y,z));
                    auto v_tmp = maxwellBoltzmannDistributedVelocity(v_bolz, dims);
                    Eigen::Vector3d v { v_tmp[0], v_tmp[1], v_tmp[2] };
                    buffer.emplace_back(pos, (body.start_velocity +  v), body.mass, typeID);
                }
            }
        }
    }

    void generateParticle(Eigen::Vector3d& x, Eigen::Vector3d& v, double m, std::list<Particle>& buffer){
        buffer.emplace_back(x, v, m, 0);
    }

    int getNextBodyID() {
        return bodyID++;
    }
}