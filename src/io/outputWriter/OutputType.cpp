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
        default:
            return "unknown";
    }
}
