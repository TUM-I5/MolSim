//
// Created by Alp Kaan Aksu on 01.11.23.
//

#include <iostream>

#include "nlohmann/json.hpp"

#include "Simulation.h"
#include "io/reader/FileReader.h"
#include "io/reader/JSONReader.h"
#include "io/outputWriter/Writer.h"
#include "io/outputWriter/VTKWriter.h"
#include "io/outputWriter/XYZWriter.h"
#include "utils/MaxwellBoltzmannDistribution.h"
#include <spdlog/spdlog.h>

#include "models/LinkedCellParticleContainer.h"

using json = nlohmann::json;

Simulation::Simulation(const std::string &filepath) {
    json definition = JSONReader::readFile(filepath);


    if (definition["simulation"]["particle_container"]["type"] == "basic") {
        particles = std::make_shared<ParticleContainer>();
    } else if (definition["simulation"]["particle_container"]["type"] == "linked_cell") {
        particles = std::make_shared<LinkedCellParticleContainer>(
            definition["simulation"]["particle_container"]["dimensions"][0],
            definition["simulation"]["particle_container"]["dimensions"][1],
            definition["simulation"]["particle_container"]["dimensions"][2],
            definition["simulation"]["particle_container"]["cell_size"]
        );
    }

    endTime = definition["simulation"]["end_time"];
    deltaT = definition["simulation"]["time_delta"];
    deltaT = definition["simulation"]["time_delta"];
    videoDuration = definition["simulation"]["video_duration"];
    fps = definition["simulation"]["frame_rate"];
    out = definition["simulation"]["output_path"];
    in = filepath;
    outputType = outputWriter::stringToOutputType(definition["simulation"]["output_type"]);

    particles->add(definition["objects"]);

    if (definition["simulation"]["model"] == "basic") {
        model = Model::basicModel(deltaT);
    } else if (definition["simulation"]["model"] == "lennard_jones") {
        model = Model::lennardJonesModel(
            deltaT,
            definition["simulation"]["epsilon"],
            definition["simulation"]["sigma"]
        );
    }
}

Simulation::Simulation(Model model, double endTime, double deltaT, int videoDuration, int fps, const std::string& in, std::string out, outputWriter::OutputType outputType)
        : endTime(endTime), deltaT(deltaT), videoDuration(videoDuration), fps(fps), in(in), out(std::move(out)), model(std::move(model)), outputType(outputType) {

    FileReader::readFile(*particles, in);
}

void Simulation::run() {
    outputWriter::prepareOutputFolder(out);

    double current_time = 0;

    int iteration = 0;

    int plotInterval = static_cast<int>((endTime / deltaT) / (videoDuration * fps));

    // Avoid division by zero
    if (plotInterval == 0) {
        plotInterval = 30;
    }

    auto resetForce = Model::resetForceFunction();
    auto force = model.forceFunction();
    auto position = model.positionFunction();
    auto velocity = model.velocityFunction();

    // Calculate initial force to avoid starting with 0 force
    particles->applyToAllPairsOnce(force);

    // Brownian Motion for all particles
    particles->applyToAll([](Particle &p) {
        p.setV(p.getV() + maxwellBoltzmannDistributedVelocity(0.1, 3));
    });

    int lastOutput = 0;

    // for this loop, we assume: current x, current f and current v are known
    while (current_time < endTime) {
        // calculate new x
        // Try to cast to LinkedCellParticleContainer
        auto linkedCellParticleContainer = dynamic_cast<LinkedCellParticleContainer*>(particles.get());

        if (linkedCellParticleContainer != nullptr) {
            // particles points to a LinkedCellParticleContainer
            linkedCellParticleContainer->applyToAll(position, true);
        } else {
            particles->applyToAll(position);
        }

        // calculate new f
        particles->applyToAll(resetForce);
        particles->applyToAllPairsOnce(force);

        // calculate new v
        particles->applyToAll(velocity);

        iteration++;

        if (iteration % plotInterval == 0) {
            plotParticles(iteration);
        }

        double percentage = current_time / endTime * 100;

        std::cout << "Running simulation: [ " << percentage << " ]\r" << std::flush;

        current_time += deltaT;
    }

    spdlog::info("Running simulation: [ 100% ] Done.");
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
        case outputWriter::Disabled: {
            writer = nullptr;
            break;
        }
    }

    std::string out_name(out + "/MD");

    if (writer != nullptr) {
        writer->plotParticles(*particles, out_name, iteration);
    }
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
        << "\nOutput type: " << outputWriter::outputTypeToString(outputType)
        << "\n========================\n";

    return stream.str();
}

double Simulation::getEndTime() const {
    return endTime;
}

double Simulation::getDeltaT() const {
    return deltaT;
}

int Simulation::getVideoDuration() const {
    return videoDuration;
}

int Simulation::getFPS() const {
    return fps;
}

std::string Simulation::getInputFilePath() const {
    return in;
}

std::string Simulation::getOutputPath() const {
    return out;
}

std::shared_ptr<ParticleContainer> Simulation::getParticles() const {
    return particles;
}

Model Simulation::getModel() const {
    return model;
}

outputWriter::OutputType Simulation::getOutputType() const {
    return outputType;
}


std::ostream &operator<<(std::ostream &stream, Simulation &simulation) {
    stream << simulation.toString();
    return stream;
}