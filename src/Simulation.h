#pragma once

#include "particleModel/ParticleContainer.h"
#include "particleModel/Model.h"
#include "outputWriter/XYZWriter.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"


namespace Simulation {

    extern std::shared_ptr<spdlog::logger> simulation_log;

    void runSimulation(ParticleContainer &particleContainer, double end_time, double delta_t);

    /**
     * @brief plot the particles to a xyz-file
     *
     * @param None
     * @return None
     */
    void plotParticles(ParticleContainer &particleContainer, int iteration);
} // namespace Simulation