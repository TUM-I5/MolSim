/*
 * InputFacade.h
 *
 *  Created on: 17.11.2022
 *      Author: borisov
 */

#pragma once

#include "./FileReader.h"
#include "./CuboidInputReader.h"

/**
 * @brief Combines multiple readers to generate particles for the simulation
 */
class InputFacade
{
private:
    std::unique_ptr<InputReader> fileReader;
    std::unique_ptr<InputReader> cuboidInputReader;

    /**
     * a spdlog logger which logs construction and destruction of particles
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:
    /**
     * @brief Construct a new input Facade object and initialize the input readers
     *
     */
    InputFacade();

    ~InputFacade();

    /**
     * @brief Checks the input file and delegates the method call to the right input reader
     *
     * @param particleContainer The container which will be passed to the InputReaders
     * @param filename The pointer to the file which will be passed to the InputReaders
     */
    void readInput(ParticleContainer &particleContainer, const char *filename);
};