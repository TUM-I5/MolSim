#pragma once

#include "./XYZWriter.h"
#include "./VTKWriter.h"

/*
* @brief: Facade which handles all the output functionalities
*/

class OutputFacade {
    private: 
        std::list<Particle> particles;
        outputWriter::XYZWriter xyzWriter;
        outputWriter::VTKWriter vtkWriter;


    public: 
        OutputFacade(std::list<Particle> particles);

        void outputXYZ(int iteration);

        void outputVTK(int iteration);
};