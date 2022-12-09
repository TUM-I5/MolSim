#include "Particle.h"
#include "ParticleGenerator.h"
#include "Body.h"
#include "utils/MaxwellBoltzmannDistribution.h"

#include <iostream>

namespace ParticleGenerator {
    int bodyID = 1;

	void generateCuboid(struct Body& body, double v_bolz, std::list<Particle>& buffer, int dims, double sigma, double epsilon){ //thermal friction hardcoded to 0.1, is that what we want to do?
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
                    buffer.back().setSigma(sigma);
                    buffer.back().setEpsilon(epsilon);
                }
            }
        }
    }

    //this implementation is basically cutting a sphere out of a cuboid. that is obviously fine and asymptotically not worse than the best solution but still..
    //there might be a smarter way to do this
    void generateSphere(struct Body& body, double v_bolz, std::list<Particle>& buffer, int dims, double sigma, double epsilon){
        struct Body bodycopy(body); //we do configurate some parameters so better copy it.. we don't want weird side effects

        if(body.shape != sphere){
            std::cerr<<"generateSphere does not work for shapes that aren't Spheres";
        }

        //configuration stuff:
            //this should be the case in a well-formed input anyway:
        bodycopy.dimensions[1] = bodycopy.dimensions[0];
        bodycopy.dimensions[2] = bodycopy.dimensions[0];

        if(dims == 2){
            bodycopy.dimensions[2] = 0;
        }

        int typeID = getNextBodyID();
        for (size_t x = 0; x <= bodycopy.dimensions[0]; x++)
        {
            for (size_t y = 0; y <= bodycopy.dimensions[1]; y++)
            {
                for (size_t z = 0; z <= bodycopy.dimensions[2]; z++)
                {
                    if(x*x + y*y + z*z <= bodycopy.dimensions[0]*bodycopy.dimensions[0]){  //bodycopy.dimensions[0] is radius measured in particle-distances

                        //we only need to generate 1/8 of a sphere 8 times.. which is what we are doing here... it looks very ugly and seems very inefficient
                        /*std::vector<Eigen::Vector3d> pos{bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(x,y,z))};
                        if(x!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(-x,  y, z)));}
                        if(y!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d( x, -y, z)));}
                        if(z!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d( x,  y,-z)));}
                        if(x!=0 && y!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(-x,-y,z)));}
                        if(x!=0 && z!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(-x,y,-z)));}
                        if(y!=0 && z!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(x,-y,-z)));}
                        if(x!=0 && y!=0 && z!=0){{pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(-x,-y,-z)));}}*/

                        std::vector<Eigen::Vector3d> pos{bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(x,y,z))};
                        if(x!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(-(double)x,          y,         z)));}
                        if(y!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(         x, -(double)y,         z)));}
                        if(z!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(         x,          y,-(double)z)));}
                        if(x!=0 && y!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(-(double)x,-(double)y,         z)));}
                        if(x!=0 && z!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(-(double)x,         y,-(double)z)));}
                        if(y!=0 && z!=0){pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(         x,-(double)y,-(double)z)));}
                        if(x!=0 && y!=0 && z!=0){{pos.emplace_back(bodycopy.fixpoint + (bodycopy.distance * Eigen::Vector3d(-(double)x,-(double)y,-(double)z)));}}

                        for(auto p : pos){
                            auto v_tmp = maxwellBoltzmannDistributedVelocity(v_bolz, dims);
                            Eigen::Vector3d v { v_tmp[0], v_tmp[1], v_tmp[2] };
                            buffer.emplace_back(p, (body.start_velocity +  v), body.mass, typeID);
                            buffer.back().setSigma(sigma);
                            buffer.back().setEpsilon(epsilon);
                        }
                    }
                }
            }
        }
    }


    void generateParticle(Eigen::Vector3d& x, Eigen::Vector3d& v, double m, std::list<Particle>& buffer, double sigma, double epsilon){
        buffer.emplace_back(x, v, m, 0);
        buffer.back().setSigma(sigma);
        buffer.back().setEpsilon(epsilon);
    }

    int getNextBodyID() {
        return bodyID++;
    }
}