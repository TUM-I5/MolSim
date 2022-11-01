/*
Implementation of the OutputFacade
*/

#include "./OutputFacade.h"

OutputFacade::OutputFacade(std::list<Particle> particles) {
    this->particles = particles;
}

/*
* For the output to work a folder named "outputXYZ" must be present in the build folder
*/
void OutputFacade::outputXYZ(int iteration) {
    std::string out_name("outputXYZ/MD_xyz");
    xyzWriter.plotParticles(particles, out_name, iteration);
}

/*
* For the output to work a folder named "outputVTK" must be present in the build folder
*/
void OutputFacade::outputVTK(int iteration) {
    std::string out_name("outputVTK/MD_vtk");
    vtkWriter.initializeOutput(particles.size());

    for (auto &p : particles) {
        vtkWriter.plotParticle(p);
    }
    vtkWriter.writeFile(out_name, iteration);
}