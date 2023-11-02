//
// Created by Feryal Ezgi on 29.10.2023.
//

#pragma once

#include <memory>
#include "models/ParticleContainer.h"

namespace outputWriter {
    enum OutputType { VTK, XYZ };

    class Writer {

    public:
        Writer() = default;

        virtual ~Writer() = default;

        virtual void plotParticles(ParticleContainer &particles, const std::string &filename, int iteration) = 0;
    };
}
