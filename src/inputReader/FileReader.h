/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "./InputReader.h"

/**
 * @brief Implementation to read input from a file
 */
class FileReader : public InputReader
{

public:
    FileReader();
    virtual ~FileReader();

    /**
     * @brief Function to read the input from a file, in this case a text file
     * @param programParameters The reference to a wrapper for the programParameters, when reading the input, new particles can be added to the container inside
     * @param filename The pointer to the filename of the input file
     */
    virtual void readInput(ProgramParameters &programParameters, const char *filename);
};
