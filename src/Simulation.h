//
// Created by Alp Kaan Aksu on 01.11.23.
//

#pragma once

#include "string"
#include "functional"
#include "models/Model.h"
#include "models/ParticleContainer.h"
#include "io/outputWriter/Writer.h"
#include "io/outputWriter/OutputType.h"

/**
 * Simulation class
 * Contains all information needed to run a simulation
 */
class Simulation {
private:
    double endTime;
    double deltaT;
    int videoDuration;
    int fps;
    std::string in;
    std::string out;
    ParticleContainer particles;
    Model model;
    outputWriter::OutputType outputType;

public:
    /**
     * Constructor for a Simulation object
     * @param filepath
     */
    Simulation(const std::string &filepath);

    /**
     * Constructor for a Simulation object
     *
     * @param model Model to be used for the simulation (determines the calculation )
     * @param endTime End time for the simulation
     * @param deltaT Time delta for the simulation (time difference between two steps)
     * @param videoDuration Duration of the ParaView animation in seconds
     * @param fps Frames per second for the ParaView animation
     * @param in File to read particles from
     * @param out Folder for output files
     * @param outputType Output file type (XYZ or VTK)
     */
    Simulation(Model model, double endTime, double deltaT, int videoDuration, int fps, const std::string& in, std::string out, outputWriter::OutputType outputType);

    /**
     * Run the simulation
     */
    void run();

    /**
     * Plot all particles
     *
     * @param iteration Iteration number
     */
    void plotParticles(int iteration);

    /**
     * Return the description of a Simulation object as a string
     *
     * @return String value of Simulation
     */
    std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Simulation &simulation);