#include "XMLFileReader.h"

#include <optional>

#include "io/logger/Logger.h"

SimulationParams XMLFileReader::readConfiguration(const std::string& filepath, ParticleContainer& particle_container) const {
    if (filepath.empty()) {
        Logger::logger->error("No input file specified.");
        exit(-1);
    }

    try {
        std::unique_ptr< ::configuration> config = configuration_(filepath);

        Logger::logger->info("Successfully parsed XML file.");
        Logger::logger->info("FPS: {}", config->fps().get());

        auto params = SimulationParams();

        return params;

    } catch (const xml_schema::exception& e) {
        Logger::logger->error("Error parsing XML file: {}", e.what());

        throw CustomFileReader::FileFormatException();
    }
}