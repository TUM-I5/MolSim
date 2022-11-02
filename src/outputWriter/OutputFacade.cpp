/*
Implementation of the OutputFacade
*/

#include "./OutputFacade.h"

OutputFacade::OutputFacade(ParticleContainer &particleContainer) {
    this->particleContainer = particleContainer;
}

/*
* For the output to work a folder named "outputXYZ" must be present in the build folder
*/
void OutputFacade::outputXYZ(int iteration) {
    std::string out_name("outputXYZ/MD_xyz");
    xyzWriter.plotParticles(particleContainer.getParticles(), out_name, iteration);
}

/*
* For the output to work a folder named "outputVTK" must be present in the build folder
*/
void OutputFacade::outputVTK(int iteration) {
    std::string out_name("outputVTK/MD_vtk");
    vtkWriter.initializeOutput(particleContainer.size());

    for (auto &p : particleContainer.getParticles()) {
        vtkWriter.plotParticle(p);
    }
    vtkWriter.writeFile(out_name, iteration);
}