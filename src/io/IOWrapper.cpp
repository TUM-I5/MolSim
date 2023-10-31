#include "IOWrapper.h"

void IOWrapper::readFile(std::string& filename, ParticleContainer& particle_container) {
    file_reader.readFile(filename, particle_container);
}

void IOWrapper::writeFile(std::string& filename, int iteration, ParticleContainer& particle_container) {
    vtk_writer.initializeOutput(particle_container.size());

    for (Particle& particle : particle_container) {
        vtk_writer.plotParticle(particle);
    }

    vtk_writer.writeFile(filename, iteration);
}