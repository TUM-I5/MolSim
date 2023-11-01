//
// Created by Feryal Ezgi on 29.10.2023.
//

#ifndef PSEMOLDYN_GROUPF_WRITER_H
#define PSEMOLDYN_GROUPF_WRITER_H

#include <memory>
#include "VTKWriter.h"
#include "XYZWriter.h"
#include "models/Particle.h"

namespace outputWriter {
    class Writer {
    private:
        std::unique_ptr<VTKWriter> vtkWriter;
        std::unique_ptr<XYZWriter> xyzWriter;

    public:
        Writer();

        void initializeOutput(int numParticles);

        void plotParticle(Particle &p);

        void writeFile(const std::string &filename, int iteration);

        void plotParticles(std::list<Particle> particles, const std::string &filename, int iteration);
    };
}
#endif //PSEMOLDYN_GROUPF_WRITER_H
