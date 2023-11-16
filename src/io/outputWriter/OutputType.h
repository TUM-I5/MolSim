//
// Created by Alp Kaan Aksu on 16.11.23.
//

#pragma once

#include <string>

namespace outputWriter {
    /**
     * Output file type (VTK or XYZ)
     */
    enum OutputType { VTK, XYZ, Disabled };

    /**
     * @brief Convert an OutputType to a string
     *
     * @param outputType
     * @return String representation of OutputType
     */
    std::string outputTypeToString(OutputType outputType);

    /**
     * @brief Convert a string to an OutputType
     * @param outputType string representation of OutputType
     * @return OutputType enum value corresponding to the given string
     */
    OutputType stringToOutputType(const std::string& outputType);
}