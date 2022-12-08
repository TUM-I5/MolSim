/*
 * XMLInputReader.cpp
 *
 *  Created on: 07.12.2022
 *      Author: wohlrapp
 */

#include "./XMLInputReader.h"
#include "../model/Sphere.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../utils/ArrayUtils.h"
#include "../utils/ParticleGenerator.h"
#include "../xsd/Simulation.cxx"

#include <string>
#include <fstream>
#include <sstream>

void XMLInputReader::readInput(ProgramParameters &programParameters, const char *filename)
{

    try
    {
        getLogicLogger()->info("Before call to parse document"); 
        std::shared_ptr<simulation_t> xml (simulation(filename));
        getLogicLogger()->info("After call to parse document"); 

        programParameters.setEndTime(xml->end_time());
        programParameters.setDeltaT(xml->delta_t());
        programParameters.setSigma(xml->sigma());
        programParameters.setEpsilon(xml->epsilon());
        programParameters.setCutoff(xml->cutoff());
        getLogicLogger()->info("parsed all single line parameters"); 


        std::array<int, 3> domain = {0, 0, 0};
        simulation_t::domain_type d = xml->domain();
        domain[0] = d.d1();
        domain[1] = d.d2();
        domain[2] = d.d3();
        programParameters.setDomain(domain);

        getLogicLogger()->info("parsed domain"); 


        for (simulation_t::file_name_const_iterator i(xml->file_name().begin()); i != xml->file_name().end(); i++)
        {
            std::string filename = i->substr(0, i->length());
            std::string path = "../input/";
            path = path.append(filename);
            
        }

        getLogicLogger()->info("parsed file"); 

        for (simulation_t::cuboid_const_iterator i(xml->cuboid().begin()); i != xml->cuboid().end(); i++)
        {
            std::array<double, 3> position;
            simulation_t::cuboid_traits::type::position_type p = i->position();
            position[0] = p.x();
            position[1] = p.y();
            position[2] = p.z();

            getLogicLogger()->info("parsed position"); 


            std::array<double, 3> velocity;
                                    getLogicLogger()->info("created velocity"); 

            simulation_t::cuboid_traits::type::velocity_type v = i->velocity();
                                  getLogicLogger()->info("created velocity"); 

            velocity[0] = v.x();
            velocity[1] = v.y();
            velocity[2] = v.z();

                        getLogicLogger()->info("parsed velocity"); 


            std::array<int, 3> dimensions;
                                    getLogicLogger()->info("created dimnesion"); 

            simulation_t::cuboid_traits::type::dimensions_type dim = i->dimensions();
            dimensions[0] = dim.x();
            dimensions[1] = dim.y();
            dimensions[2] = dim.z();

                        getLogicLogger()->info("parsed dimensions"); 


            double h = i->h();
            double m = i->mass();
            double meanV = i->meanV();
            int type = i->type();

                        getLogicLogger()->info("Set one line parameters"); 


            std::unique_ptr<Cuboid> cuboid = std::make_unique<Cuboid>(Cuboid(position, dimensions, h, m, velocity, meanV, type));
            ParticleGenerator::generateCuboid(*programParameters.getParticleContainer(), *cuboid);
            getLogicLogger()->info("parsed cuboid"); 
        }

        for (simulation_t::sphere_const_iterator i(xml->sphere().begin()); i != xml->sphere().end(); i++)
        {
            std::array<double, 3> center;
            simulation_t::sphere_traits::type::center_type c = i->center();
            center[0] = c.x();
            center[1] = c.y();
            center[2] = c.z();

            std::array<double, 3> velocity;
            simulation_t::sphere_traits::type::velocity_type v = i->velocity();
            velocity[0] = v.x();
            velocity[1] = v.y();
            velocity[2] = v.z();

            int r = i->r();
            double h = i->h();
            double m = i->mass();
            double meanV = i->meanV();
            int type = i->type();

            std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(Sphere(center, r, h, m, velocity, meanV, type));
            ParticleGenerator::generateSphere(*programParameters.getParticleContainer(), *sphere);
            getLogicLogger()->info("parsed sphere"); 
        }

        getLogicLogger()->info("Finished parsing"); 
    }
    catch (const xml_schema::exception &e)
    {
        getLogicLogger()->error(e.what());
    }
}