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
            if (lower_left_front_corner[0] < 0 || lower_left_front_corner[1] < 0 || lower_left_front_corner[2] < 0){
                Logger::logger->error("Cuboid position must be positive");
                exit(-1);
            }
            std::array<long long int, 3> grid_dimensions{config->cuboid()[i].grid_dim().dimx(), config->cuboid()[i].grid_dim().dimy(), config->cuboid()[i].grid_dim().dimz()};
            if(grid_dimensions[0] < 0 || grid_dimensions[1] < 0 || grid_dimensions[2] < 0){
                Logger::logger->error("Cuboid grid dimensions must be positive");
                exit(-1);
            }
            if(grid_dimensions[0] > 2147483647 || grid_dimensions[1] > 2147483647 || grid_dimensions[2] > 2147483647){
                Logger::logger->error("Cuboid grid dimensions must fit in an integer");
                exit(-1);
            }
            std::array<int,3> grid_dimensions_int{static_cast<int>(grid_dimensions[0]), static_cast<int>(grid_dimensions[1]), static_cast<int>(grid_dimensions[2])};
            std::array<double, 3> initial_velocity{config->cuboid()[i].velocity().x(), config->cuboid()[i].velocity().y(), config->cuboid()[i].velocity().z()};
            if(initial_velocity[0] < 0 || initial_velocity[1] < 0 || initial_velocity[2] < 0){
                Logger::logger->error("Cuboid initial velocity must be positive");
                exit(-1);
            }
            Logger::logger->info("Position : {} {} {}", lower_left_front_corner[0], lower_left_front_corner[1], lower_left_front_corner[2]);

            auto spawner = CuboidSpawner(lower_left_front_corner, grid_dimensions_int, config->cuboid()[i].grid_spacing(), config->cuboid()[i].mass(), initial_velocity, config->cuboid()[i].type(), config->cuboid()[i].temperature());
            particle_container.reserve(particle_container.size() + static_cast<long>(grid_dimensions[0]) * grid_dimensions[1] * grid_dimensions[2]);
            spawner.spawnParticles(particle_container);
        }


        return params;

    } catch (const xml_schema::exception& e) {
        Logger::logger->error("Error parsing XML file: {}", e.what());
        throw FileFormatException();
    }
}