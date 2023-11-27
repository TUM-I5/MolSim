#include "XMLFileReader.h"

#include <optional>

#include "io/input/xml/parser/XSDTypeAdapter.h"
#include "io/logger/Logger.h"
#include "io/particle_spawners/CuboidSpawner.h"

SimulationParams XMLFileReader::readFile(const std::string& filepath, ParticleContainer& particle_container) const {
    try {
        std::unique_ptr< ::configuration> config = configuration_(filepath);

        // get the container type at runtime
        int container_type;
        double domain_size = 10;
        double cutoff_radius = 1;
        if (config->linkedcells_container().present()) {
            container_type = 1;
            domain_size = config->linkedcells_container().get().domain_size();
            cutoff_radius = config->linkedcells_container().get().cutoff_radius();
        } else {
            container_type = 2;
        }

        const std::string& output_dir_path = "";
        const std::string& log_level = "";

        // set output format
        FileOutputHandler::OutputFormat format = FileOutputHandler::OutputFormat::VTK;
        if (config->output_format() == "VTK") {
            format = FileOutputHandler::OutputFormat::VTK;
        } else if (config->output_format() == "XYZ") {
            format = FileOutputHandler::OutputFormat::XYZ;
        } else {
            Logger::logger->error("Output format not implemented. Has to be VTK or XYZ.");
            exit(1);
        }

        auto params = SimulationParams(filepath, output_dir_path, config->delta_t(), config->end_time(), config->fps(),
                                       config->video_length(), log_level, container_type, domain_size, cutoff_radius, format);

        for (auto xsd_cuboid : config->cuboid()) {
            auto spawner = XSDTypeAdapter::convertToCuboidSpawner(xsd_cuboid);

            particle_container.reserve(particle_container.size() + spawner.getEstimatedNumberOfParticles());

            spawner.spawnParticles(particle_container);
        }

        // TODO Adapter for spawning a sphere

        return params;
    } catch (const xml_schema::exception& e) {
        Logger::logger->error("Error parsing XML file: {}", e.what());
        throw FileFormatException();
    }
}