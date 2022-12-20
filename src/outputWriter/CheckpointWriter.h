/*
 * VTKWriter.h
 *
 *  Created on: 14.12.2022
 *      Author: wohlrapp
 */

#pragma once

#include "../model/ParticleContainer.h"

namespace outputWriter
{
    /**
     * This class implements the functionality to generate a checkpoint file
     */
    class CheckpointWriter
    {

    public:
        CheckpointWriter();

        virtual ~CheckpointWriter();

        /**
         * write the particles from the particle container to a file
         */
        void writeCheckpoint(ParticleContainer *particleContainer);

    private:
        /**
         * A spdlog logger, which logs the logic of the program flow
         */
        std::shared_ptr<spdlog::logger> _logicLogger;
        /**
         * a speedlog logger which logs construction and destruction of particles
         */
        std::shared_ptr<spdlog::logger> _memoryLogger;
    };
}