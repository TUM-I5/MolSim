#pragma once

#include "ParticleContainer.h"
#include "Model.h"
#include "outputWriter/XYZWriter.h"

namespace Simulator {
    void runSimulation(ParticleContainer &particleContainer, double end_time, double delta_t);

    /**
     * @brief plot the particles to a xyz-file
     *
     * @param None
     * @return None
     */
    void plotParticles(ParticleContainer &particleContainer, int iteration);
} // namespace Simulator