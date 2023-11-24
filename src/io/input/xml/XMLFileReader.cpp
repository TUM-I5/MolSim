#include "XMLFileReader.h"

#include <optional>

#include "io/logger/Logger.h"

SimulationParams XMLFileReader::readFile(const std::string& filepath, std::unique_ptr<ParticleContainer>& particle_container) const {
    try {
        std::unique_ptr< ::configuration> config = configuration_(filepath);

        Logger::logger->info("Successfully parsed XML file.");
        Logger::logger->info("FPS: {}", config->fps().get());

        auto params = SimulationParams();

        return params;

    } catch (const xml_schema::exception& e) {
        Logger::logger->error("Error parsing XML file: {}", e.what());
        throw FileFormatException();
    }
}