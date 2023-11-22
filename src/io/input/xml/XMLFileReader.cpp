#include "XMLFileReader.h"

#include "io/logger/Logger.h"

SimulationParams XMLFileReader ::readConfiguration(const std::string& filepath, ParticleContainer& particle_container) const {
    if (filepath.empty()) {
        Logger::logger->error("No input file specified.");
        exit(-1);
    }

    xml_schema::integer_pimpl int_p;
    configuration_pimpl configuration_p;

    configuration_p.fps_parser(int_p);

    xml_schema::document doc_p(configuration_p, "configuration", "http://www.w3.org/2001/XMLSchema-instance");

    try {
        doc_p.parse(filepath);

        Logger::logger->info("Successfully parsed XML file.");
        Logger::logger->info("FPS: {}", configuration_p.get_fps());

        auto params = SimulationParams();
        params.fps = std::stoi(configuration_p.get_fps());

        return params;

    } catch (const xml_schema::exception& e) {
        Logger::logger->error("Error parsing XML file: {}", e.what());

        throw CustomFileReader::FileFormatException();
    }
}