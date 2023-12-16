//
// Created by Alp Kaan Aksu on 01.11.23.
//

#pragma once

#include "string"
#include "functional"
#include "models/Model.h"
#include "utils/Thermostat.h"
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
    std::shared_ptr<ParticleContainer> particles;
    Model model;
    outputWriter::OutputType outputType;
    int checkpoint;
    Thermostat thermostat;

public:
    /**
     * Constructor for a Simulation object
     *
     * @note This constructor is used to read a simulation from a JSON file
     *
     * @param filepath
     */
    explicit Simulation(const std::string &filepath);

    Simulation(const std::string &filepath, const int checkpoint);

    /**
     * Constructor for a Simulation object
     *
     * @note This constructor is used to read a simulation from a legacy text file
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
    Simulation(Model model, double endTime, double deltaT, int videoDuration, int fps, const std::string &in,
               std::string out, outputWriter::OutputType outputType);

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

    double getEndTime() const;

    double getDeltaT() const;

    int getVideoDuration() const;

    int getFPS() const;

    std::string getInputFilePath() const;

    std::string getOutputPath() const;

    std::shared_ptr<ParticleContainer> getParticles() const;

    Model getModel() const;

    outputWriter::OutputType getOutputType() const;
};

std::ostream &operator<<(std::ostream &stream, Simulation &simulation);