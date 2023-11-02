//
// Created by Feryal Ezgi on 29.10.2023.
//

#pragma once

#include <memory>
#include "models/ParticleContainer.h"

namespace outputWriter {
    enum OutputType { VTK, XYZ };

    /**
     * Base class for all Writers
     */
    class Writer {

    public:

        /**
         * @brief Export all particles in their current state into a file
         *
         * @param particles
         * @param filename
         * @param iteration
         */
        virtual void plotParticles(ParticleContainer &particles, const std::string &filename, int iteration) = 0;
    };
}
