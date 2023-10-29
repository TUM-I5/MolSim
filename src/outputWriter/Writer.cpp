#include "Writer.h"

#include <utility>

namespace outputWriter {
    Writer::Writer() : vtkWriter(std::make_unique<VTKWriter>()), xyzWriter(std::make_unique<XYZWriter>()) {}

    void Writer::initializeOutput(int numParticles) {
        if (vtkWriter) {
            vtkWriter->initializeOutput(numParticles);
        }
    }

    void Writer::plotParticle(Particle& p) {
        if (vtkWriter) {
            vtkWriter->plotParticle(p);
        }
    }

    void Writer::writeFile(const std::string& filename, int iteration) {
        if (vtkWriter) {
            vtkWriter->writeFile(filename, iteration);
        }
    }

    void Writer::plotParticles(std::list<Particle> particles, const std::string& filename, int iteration) {
        if (xyzWriter) {
            //todo: std::move(particles) or particles?
            xyzWriter->plotParticles(std::move(particles), filename, iteration);
        }
    }
}
