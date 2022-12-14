
#include "./CheckpointWriter.h"
#include "../xsd/SimulationState.hxx"
#include "../utils/File.h"

#include <iostream>
#include <fstream>

namespace outputWriter
{
    CheckpointWriter::CheckpointWriter()
    {
        _logicLogger = spdlog::get("output_logger");
        _memoryLogger = spdlog::get("memory_logger");
        _memoryLogger->info("CheckpointWriter generated!");
    }

    CheckpointWriter::~CheckpointWriter()
    {
        _memoryLogger->info("CheckpointWriter destructed!");
    }

    void CheckpointWriter::writeCheckpoint(ParticleContainer *particleContainer)
    {
        simulation_state_t simulationState;
        simulation_state_t::particle_sequence &ps(simulationState.particle());

        for (auto &p : particleContainer->getActiveParticles())
        {

            particle::x_type x_xml = x(p.getX()[0], p.getX()[1], p.getX()[2]);
            particle::v_type v_xml = v(p.getV()[0], p.getV()[1], p.getV()[2]);
            particle::f_type f_xml = f(p.getF()[0], p.getF()[1], p.getF()[2]);
            particle::old_f_type old_f_xml = old_f(p.getOldF()[0], p.getOldF()[1], p.getOldF()[2]);

            ps.push_back(
                particle(p.getM(), p.getEpsilon(), p.getSigma(), p.getType(), x_xml, v_xml, f_xml, old_f_xml));
        }

        xml_schema::namespace_infomap map;
        map[""].name = "";
        map[""].schema = "../src/xsd/SimulationState.xsd";

        std::ofstream file;
        std::string filename = File::getDateTime() + "simulation_status.xml";
        file.open("../checkpoints/" + filename);
        simulation_state(file, simulationState, map);
        file.close();
    }
}