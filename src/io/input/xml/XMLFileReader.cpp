#include "XMLFileReader.h"

#include "io/logger/Logger.h"

SimulationParams XMLFileReader::readConfiguration(const std::string& filepath, ParticleContainer& particle_container) const {
    if (filepath.empty()) {
        Logger::logger->error("No input file specified.");
        exit(-1);
    }

    // set up the basic parsers
    xml_schema::integer_pimpl integer_p;
    xml_schema::decimal_pimpl decimal_p;

    // set up the positionType parser
    positionType_pimpl positionType_p;
    positionType_p.parsers(decimal_p, decimal_p, decimal_p);

    // set up the gridDimType parser
    gridDimType_pimpl gridDimType_p;
    gridDimType_p.parsers(integer_p, integer_p, integer_p);

    // set up the velocityType parser
    velocityType_pimpl velocityType_p;
    velocityType_p.parsers(decimal_p, decimal_p, decimal_p);

    // set up the cuboid parser
    cuboid_pimpl cuboid_p;
    cuboid_p.parsers(positionType_p, gridDimType_p, decimal_p, decimal_p, decimal_p, velocityType_p, integer_p);

    // Construct the configuration parser
    configuration_pimpl configuration_p;
    configuration_p.parsers(integer_p, integer_p, decimal_p, decimal_p, cuboid_p);

    xml_schema::document doc_p(configuration_p, "configuration");

    try {
        configuration_p.pre();
        doc_p.parse(filepath);
        configuration_p.post_configuration();

        Logger::logger->info("Successfully parsed XML file.");

        auto params = SimulationParams();

        return params;

    } catch (const xml_schema::exception& e) {
        Logger::logger->error("Error parsing XML file: {}", e.what());

        throw CustomFileReader::FileFormatException();
    }
}