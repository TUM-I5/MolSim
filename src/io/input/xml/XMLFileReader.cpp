#include "XMLFileReader.h"

#include <optional>

#include "io/input/xml/XSDTypeAdapter.h"
#include "io/logger/Logger.h"
#include "io/particle_spawners/CuboidSpawner.h"

SimulationParams XMLFileReader::readFile(const std::string& filepath, ParticleContainer& particle_container) const {
    try {
        std::unique_ptr< ::configuration> config = configuration_(filepath);

        Logger::logger->info("Successfully parsed XML file.Simulation parameters: ");
        Logger::logger->info("FPS: {}", config->fps().get());
        Logger::logger->info("Video length: {}", config->video_length().get());
        Logger::logger->info("End time: {}", config->end_time().get());
        Logger::logger->info("Delta t: {}", config->delta_t().get());

        if (config->fps().get() < 0) {
            Logger::logger->error("FPS must be positive");
            exit(-1);
        }
        if (config->video_length().get() < 0) {
            Logger::logger->error("Video length must be positive");
            exit(-1);
        }
        if (config->end_time().get() < 0) {
            Logger::logger->error("End time must be positive");
            exit(-1);
        }
        if (config->delta_t().get() < 0) {
            Logger::logger->error("Delta t must be positive");
            exit(-1);
        }

        const std::string& output_dir_path = "";
        const std::string& log_level = "";
        auto params = SimulationParams(filepath, output_dir_path, config->delta_t().get(), config->end_time().get(), config->fps().get(),
                                       config->video_length().get(), log_level);

        for (auto xsd_cuboid : config->cuboid()) {
            auto spawner = XMLTypeAdapter::convertToCuboidSpawner(xsd_cuboid);

            particle_container.reserve(particle_container.size() + spawner.getEstimatedNumberOfParticles());

            spawner.spawnParticles(particle_container);
        }

        return params;
    } catch (const xml_schema::exception& e) {
        Logger::logger->error("Error parsing XML file: {}", e.what());
        throw FileFormatException();
    }
}