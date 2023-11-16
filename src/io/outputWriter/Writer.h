//
// Created by Feryal Ezgi on 29.10.2023.
//

#pragma once

#include <memory>
#include "models/ParticleContainer.h"

namespace outputWriter {
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

    /**
     * @brief Creates a new empty folder at the given path
     *
     * @note If the folder already exists, it will be removed and created again
     *
     * @param path path to the output folder
     */
    void prepareOutputFolder(const std::string &path);
}
