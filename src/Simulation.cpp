//
// Created by Alp Kaan Aksu on 01.11.23.
//

#include <iostream>
#include <iomanip>

#include "Simulation.h"
#include "FileReader.h"

Simulation::Simulation(Model model, double endTime, double deltaT, int videoDuration, int fps, std::string in, std::string out)
        : model(model), endTime(endTime), deltaT(deltaT), videoDuration(videoDuration), fps(fps), in(in), out(out) {

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
        auto resetForce = model.resetForceFunction();
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

    std::string out_name(out + "/MD_vtk");

    outputWriter::VTKWriter writer;

    writer.initializeOutput(particles.count());

    particles.plot(writer);

    writer.writeFile(out_name, iteration);
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
        << "\n========================\n";

    return stream.str();
}

std::ostream &operator<<(std::ostream &stream, Simulation &simulation) {
    stream << simulation.toString();
    return stream;
}
