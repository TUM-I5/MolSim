#pragma once

#include "particleModel/ParticleContainer.h"
#include "particleModel/Model.h"
#include "outputWriter/XYZWriter.h"

namespace Simulation {
    void runSimulation(ParticleContainer &particleContainer, double end_time, double delta_t);

    /**
     * @brief plot the particles to a xyz-file
     *
     * @param None
     * @return None
     */
    void plotParticles(ParticleContainer &particleContainer, int iteration);
} // namespace Simulation