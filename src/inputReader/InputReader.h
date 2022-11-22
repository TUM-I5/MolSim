/*
 * InputReader.h
 *
 * Created: 31.10.2022
 * Author:  wohlrapp
 */

#pragma once

#include "../model/ParticleContainer.h"
#include "spdlog/spdlog.h"

#include <vector>
#include <memory>

/**
 * @brief Base class to read input, provides a shared interface
 */
class InputReader
{

private:
    /**
     * A spdlog logger, which logs the logic of the program flow
     */
    std::shared_ptr<spdlog::logger> _logicLogger;

    /**
     * a speedlog logger which logs construction and destruction of particles
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:
    /**
     * @brief Constructor for the input reader, used to initialize loggers
     */
    InputReader();

    ~InputReader();

    /**
     * @brief Abstract function to read the input from a file
     * @param particleContainer The reference to a wrapper for the particles, when reading the input, new particles can be added to that container
     * @param filename The pointer to the filename of the input file
     */
    virtual void readInput(ParticleContainer &particleContainer, const char *filename) = 0;

    const std::shared_ptr<spdlog::logger> getLogicLogger() const;
};