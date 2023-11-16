//
// Created by Alp Kaan Aksu on 16.11.23.
//

#include "OutputType.h"

std::string outputWriter::outputTypeToString(outputWriter::OutputType outputType) {
    switch (outputType) {
        case outputWriter::VTK:
            return "VTK";
        case outputWriter::XYZ:
            return "XYZ";
        case outputWriter::Disabled:
            return "Disabled";
        default:
            return "unknown";
    }
}

outputWriter::OutputType outputWriter::stringToOutputType(const std::string& outputType) {
    if (outputType == "vtk") {
        return outputWriter::VTK;
    } else if (outputType == "xyz") {
        return outputWriter::XYZ;
    } else if (outputType == "disabled") {
        return outputWriter::Disabled;
    } else {
        return outputWriter::Disabled;
    }
}
