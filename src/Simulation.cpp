//
// Created by Alp Kaan Aksu on 01.11.23.
//

#include <iostream>
#include <iomanip>
#include <utility>

#include "Simulation.h"
#include "io/reader/FileReader.h"
#include "io/outputWriter/Writer.h"
#include "io/outputWriter/VTKWriter.h"
#include "io/outputWriter/XYZWriter.h"

Simulation::Simulation(Model model, double endTime, double deltaT, int videoDuration, int fps, const std::string& in, std::string out, outputWriter::OutputType outputType)
        : endTime(endTime), deltaT(deltaT), videoDuration(videoDuration), fps(fps), in(in), out(std::move(out)), model(std::move(model)), outputType(outputType) {

    ParticleContainer particleContainer;

    FileReader::readFile(particleContainer, in);

    particles = particleContainer;
}

void Simulation::run() {
    double current_time = 0;

    int iteration = 0;

    int plotInterval = static_cast<int>((endTime / deltaT) / (videoDuration * fps));

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < endTime) {
        auto resetForce = Model::resetForceFunction();
        auto force = model.forceFunction();
        auto position = model.positionFunction();
        auto velocity = model.velocityFunction();

        // calculate new x
        particles.applyToAll(position);

        // calculate new f
        particles.applyToAll(resetForce);
        particles.applyToAllPairs(force);

        // calculate new v
        particles.applyToAll(velocity);

        iteration++;

        if (iteration % plotInterval == 0) {
            plotParticles(iteration);
            std::cout << std::fixed << std::setprecision(2) <<"Running simulation: [ " <<current_time / endTime * 100 << "% ] "  << "\r" << std::flush;
        }

        current_time += deltaT;


    }

    std::cout << "Running simulation: [ 100% ] Done.\n" << std::endl;
}

void Simulation::plotParticles(int iteration) {
    outputWriter::VTKWriter vtkWriter{};
    outputWriter::XYZWriter xyzWriter{};

    outputWriter::Writer* writer = &vtkWriter;

    switch (outputType) {
        case outputWriter::VTK: {
            writer = &vtkWriter;
            break;
        }
        case outputWriter::XYZ: {
            writer = &xyzWriter;
            break;
        }
    }

    std::string out_name(out + "/MD");

    writer->plotParticles(particles, out_name, iteration);
}

std::string Simulation::toString() const {
    std::stringstream stream;
    stream << "\n====== Simulation ======"
        << "\nEnd time: " << endTime
        << "\nTime delta: " << deltaT
        << "\nVideo duration (s): " << videoDuration
        << "\nFrames per second: " << fps
        << "\n"
        << "\nReading from: " << in
        << "\nOutput to: " << out << '/'
        << "\nOutput type: " << outputType
        << "\n========================\n";

    return stream.str();
}

std::ostream &operator<<(std::ostream &stream, Simulation &simulation) {
    stream << simulation.toString();
    return stream;
}
