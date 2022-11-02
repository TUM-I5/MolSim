#pragma once

#include "./XYZWriter.h"
#include "./VTKWriter.h"
#include "../model/ParticleContainer.h"

/*
* @brief: Facade which handles all the output functionalities
*/

class OutputFacade {
    private: 
        ParticleContainer particleContainer;
        outputWriter::XYZWriter xyzWriter;
        outputWriter::VTKWriter vtkWriter;


    public: 
        OutputFacade(ParticleContainer &particleContainer);

        void outputXYZ(int iteration);

        void outputVTK(int iteration);
};