/*
 * XMLInputReader.cpp
 *
 *  Created on: 07.12.2022
 *      Author: wohlrapp
 */

#include "./XMLInputReader.h"
#include "./InputFacade.h"
#include "../model/Sphere.h"
#include "../utils/MaxwellBoltzmannDistribution.h"
#include "../utils/ArrayUtils.h"
#include "../utils/ParticleGenerator.h"
#include "../xsd/Simulation.cxx"

#include <string>
#include <fstream>
#include <sstream>

BoundaryCondition getBoundaryCondition(std::string s)
{
    if (s == "Outflow")
        return BoundaryCondition::Outflow;

    if (s == "Reflecting")
        return BoundaryCondition::Reflecting;

    if (s == "Periodic")
        return BoundaryCondition::Periodic;

    return BoundaryCondition::None;
}

void XMLInputReader::readInput(ProgramParameters &programParameters, const char *filename)
{

    try
    {
        std::shared_ptr<simulation_t> xml(simulation(filename));
        std::shared_ptr<InputFacade> inputFacade = std::make_shared<InputFacade>();

        programParameters.setEndTime(xml->end_time());
        programParameters.setDeltaT(xml->delta_t());
        programParameters.setSigma(xml->sigma());
        programParameters.setEpsilon(xml->epsilon());
        programParameters.setCutoff(xml->cutoff());
        programParameters.setWriteFrequency(xml->writeFrequency());
        programParameters.setBaseName(xml->baseName());
        programParameters.setTempInit(xml->temp_init());
        programParameters.setBrownianMotion(xml->brownianMotion());

        std::array<double, 3> domain;
        simulation_t::domain_type d = xml->domain();
        domain[0] = d.x();
        domain[1] = d.y();
        domain[2] = d.z();
        programParameters.setDomain(domain);

        std::array<BoundaryCondition, 6> boundaries = std::array<BoundaryCondition, 6>();
        simulation_t::boundaries_type b = xml->boundaries();
        std::string boundary = b.xLeft();
        boundaries[0] = getBoundaryCondition(boundary);
        boundary = b.xRight();
        boundaries[1] = getBoundaryCondition(boundary);
        boundary = b.yBottom();
        boundaries[2] = getBoundaryCondition(boundary);
        boundary = b.yTop();
        boundaries[3] = getBoundaryCondition(boundary);
        boundary = b.zFront();
        boundaries[4] = getBoundaryCondition(boundary);
        boundary = b.zBack();
        boundaries[5] = getBoundaryCondition(boundary);
        programParameters.setBoundaries(boundaries);

        if (xml->n_thermostat().present())
        {
            programParameters.setNThermostats(xml->n_thermostat().get());
        }

        if (xml->temp_target().present())
        {
            programParameters.setTempTarget(xml->temp_target().get());
        }

        if (xml->delta_temp().present())
        {
            programParameters.setDeltaTemp(xml->delta_temp().get());
        }

        if (xml->g_grav().present())
        {
            programParameters.setGGrav(xml->g_grav().get());
        }

        for (simulation_t::file_name_const_iterator i(xml->file_name().begin()); i != xml->file_name().end(); i++)
        {
            std::string filename = i->substr(0, i->length());
            std::string path = "../input/";
            path = path.append(filename);
            inputFacade->readInput(programParameters, path.c_str());
        }

        for (simulation_t::cuboid_const_iterator i(xml->cuboid().begin()); i != xml->cuboid().end(); i++)
        {
            std::array<double, 3> position;
            simulation_t::cuboid_traits::type::position_type p = i->position();
            position[0] = p.x();
            position[1] = p.y();
            position[2] = p.z();

            std::array<double, 3> velocity;

            simulation_t::cuboid_traits::type::velocity_type v = i->velocity();

            velocity[0] = v.x();
            velocity[1] = v.y();
            velocity[2] = v.z();

            std::array<int, 3> dimensions;

            simulation_t::cuboid_traits::type::dimensions_type dim = i->dimensions();
            dimensions[0] = dim.x();
            dimensions[1] = dim.y();
            dimensions[2] = dim.z();

            double h = i->h();
            double m = i->mass();
            double meanV = i->meanV();
            double epsilon = i->epsilon();
            double sigma = i->sigma();
            int type = i->type();

            std::unique_ptr<Cuboid> cuboid = std::make_unique<Cuboid>(Cuboid(position, dimensions, h, m, velocity, meanV, epsilon, sigma, type));
            ParticleGenerator::generateCuboid(*programParameters.getParticleContainer(), *cuboid);
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
            double epsilon = i->epsilon();
            double sigma = i->sigma();
            int type = i->type();

            std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(Sphere(center, r, h, m, velocity, meanV, epsilon, sigma, type));
            ParticleGenerator::generateSphere(*programParameters.getParticleContainer(), *sphere);
        }
    }
    catch (const xml_schema::exception &e)
    {
        getLogicLogger()->error(e.what());
    }
}