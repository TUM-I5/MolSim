/*
 * OutputFacade.cpp
 *
 * Created: 01.11.2022
 * Author:  borisov
 */

#include "./OutputFacade.h"

#include <iostream>

OutputFacade::OutputFacade(std::shared_ptr<ParticleContainer> particleContainer, std::string baseName)
{
    this->particleContainer = particleContainer;
    this->baseName = baseName; 

    _logicLogger = spdlog::get("output_logger");
    _memoryLogger = spdlog::get("memory_logger");
    _memoryLogger->info("OutputFacade generated!");

    // deleting folders and recreating them, so they are empty for every run of the simulation
    // deleting the folders
    removeDirectory(baseName);

    // creating folders which are needed for output
    createDirectory(baseName);
}

OutputFacade::~OutputFacade()
{
    _memoryLogger->info("OutputFacade destructed!");
}

void OutputFacade::outputXYZ(int iteration)
{
    std::string out_name(baseName + "/MD_xyz");
    xyzWriter.plotParticles((*particleContainer).getActiveParticles(), out_name, iteration);
}

void OutputFacade::outputVTK(int iteration)
{
    std::string out_name(baseName + "/MD_vtk");
    vtkWriter.initializeOutput((*particleContainer).size());

    for (auto &p : (*particleContainer).getActiveParticles())
    {
        vtkWriter.plotParticle(p);
    }
    vtkWriter.writeFile(out_name, iteration);
}

void OutputFacade::createDirectory(std::string path)
{
    // Unix command for creating folders
    std::string command = "mkdir -p " + path;
    // feeding the command to the commandline processor
    if (std::system(command.c_str()) != 0)
    {
        // return message if there was an error creating the folder
        _logicLogger->info("Folder {} could not be created!", path);
        _logicLogger->info("No output for this folder will be written!");
    }
}

void OutputFacade::removeDirectory(std::string path)
{
    // Unix command for deleting folders
    std::string command = "rm -rf " + path;
    // feeding the command to the commandline processor
    if (std::system(command.c_str()) != 0)
    {
        // return message if there was an error deleting the folder
        _logicLogger->info("Folder {} could not be deleted!", path);
    }
}