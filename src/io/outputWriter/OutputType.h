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
     * Convert an OutputType to a string
     * @param outputType
     * @return String representation of OutputType
     */
    std::string outputTypeToString(OutputType outputType);

    OutputType stringToOutputType(const std::string& outputType);
}