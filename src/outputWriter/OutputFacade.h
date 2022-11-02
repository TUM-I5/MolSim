/*
 * OutputFacade.h
 * 
 * Created: 01.11.2022
 * Author:  borisov 
*/

#pragma once

#include "./XYZWriter.h"
#include "./VTKWriter.h"
#include "../model/ParticleContainer.h"

/**
* @brief: Facade which handles all the output functionalities
*/
class OutputFacade {
    private: 
        ParticleContainer* particleContainer;
        outputWriter::XYZWriter xyzWriter;
        outputWriter::VTKWriter vtkWriter;

        /**
         * @brief create a directory
         * 
         * @param path of the directory to be created
         */
        void createDirectory(std::string path);

        /**
         * @brief deletes a directory
         * 
         * @param path of the directory to be deleted
         */
        void removeDirectory(std::string path);


    public: 
        /**
         * @brief Constructs a new Output Facade object and creates folders which are needed for output
         * 
         * @param particleContainer particles whose data will be written in the files
         */
        OutputFacade(ParticleContainer* particleContainer);

        /**
        * @brief wirtes .xyz files, containing the simulation's data, to the build/outputXYZ/ folder
        * @param iteration number of the iteration, which is used to generate a unique filename
        */
        void outputXYZ(int iteration);


        /**
        * @brief wirtes .vtu files, containing the simulation's data, to the build/outputVTK/ folder
        * @param iteration number of the iteration, which is used to generate a unique filename
        */
        void outputVTK(int iteration);
};