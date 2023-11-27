#include "XMLFileReader.h"

#include <optional>

#include "io/input/xml/parser/XSDTypeAdapter.h"
#include "io/logger/Logger.h"
#include "io/particle_spawners/CuboidSpawner.h"

SimulationParams XMLFileReader::readFile(const std::string& filepath, ParticleContainer& particle_container) const {
    try {
        std::unique_ptr< ::configuration> config = configuration_(filepath);

        // get the container type at runtime
        std::variant<SimulationParams::DirectSumType, SimulationParams::LinkedCellsType> container_type;
        if (config->linkedcells_container().present()) {
            auto container_data = config->linkedcells_container().get();
            std::array<double, 3> domain_size{container_data.domain_size().x(), container_data.domain_size().y(),
                                              container_data.domain_size().z()};
            container_type = SimulationParams::LinkedCellsType(domain_size, container_data.cutoff_radius());
        } else {
            container_type = SimulationParams::DirectSumType();
        }

        const std::string output_dir_path = "";

        SimulationParams params = SimulationParams(filepath, output_dir_path, config->delta_t(), config->end_time(), config->fps(),
                                                   config->video_length(), container_type, "vtk");

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