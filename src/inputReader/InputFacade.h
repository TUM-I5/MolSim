/*
 * InputFacade.h
 *
 *  Created on: 17.11.2022
 *      Author: borisov
 */

#pragma once

#include "./FileReader.h"
#include "./CuboidInputReader.h"
#include "./SphereInputReader.h"
#include "./XMLInputReader.h"
#include "../model/ProgramParameters.h"

/**
 * @brief Combines multiple readers to generate particles for the simulation
 */
class InputFacade
{
private:
    std::unique_ptr<InputReader> fileReader;
    std::unique_ptr<InputReader> cuboidInputReader;
    std::unique_ptr<InputReader> sphereInputReader;
    std::unique_ptr<InputReader> xmlInputReader; 

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
     * @param programParameters The reference to a wrapper for the programParameters, when reading the input, new particles can be added to the container inside
     * @param filename The pointer to the file which will be passed to the InputReaders
     */
    void readInput(ProgramParameters &programParameters, const char *filename);
};