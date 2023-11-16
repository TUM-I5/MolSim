//
// Created by Alp Kaan Aksu on 16.11.23.
//

#pragma once

#include <string>

namespace outputWriter {
    enum OutputType { VTK, XYZ };

    std::string outputTypeToString(OutputType outputType);
}