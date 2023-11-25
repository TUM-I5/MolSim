#include "XMLFileReader.h"

#include <optional>

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


        const std::string& output_dir_path = "";
        const std::string& log_level = "";
        auto params = SimulationParams(filepath, output_dir_path, config->delta_t().get(), config->end_time().get(),
                                       config->fps().get(), config->video_length().get(), log_level);

        for(int i = 0;i<config->cuboid().size();i++){
            std::array<double, 3> lower_left_front_corner{config->cuboid()[i].position().x(), config->cuboid()[i].position().y(), config->cuboid()[i].position().z()};
            std::array<int, 3> grid_dimensions{config->cuboid()[i].grid_dim().dimx(), config->cuboid()[i].grid_dim().dimy(), config->cuboid()[i].grid_dim().dimz()};
            std::array<double, 3> initial_velocity{config->cuboid()[i].velocity().x(), config->cuboid()[i].velocity().y(), config->cuboid()[i].velocity().z()};
            Logger::logger->info("Position : {} {} {}", lower_left_front_corner[0], lower_left_front_corner[1], lower_left_front_corner[2]);

            auto spawner = CuboidSpawner(lower_left_front_corner, grid_dimensions, config->cuboid()[i].grid_spacing(), config->cuboid()[i].mass(), initial_velocity, config->cuboid()[i].type(), config->cuboid()[i].temperature());
            particle_container.reserve(particle_container.size() + static_cast<long>(grid_dimensions[0]) * grid_dimensions[1] * grid_dimensions[2]);
            spawner.spawnParticles(particle_container);
        }


        return params;

    } catch (const xml_schema::exception& e) {
        Logger::logger->error("Error parsing XML file: {}", e.what());
        throw FileFormatException();
    }
}