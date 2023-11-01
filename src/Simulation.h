//
// Created by Alp Kaan Aksu on 01.11.23.
//

#ifndef PSEMOLDYN_GROUPF_SIMULATION_H
#define PSEMOLDYN_GROUPF_SIMULATION_H

#include "string"
#include "functional"
#include "models/Model.h"
#include "models/ParticleContainer.h"

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

public:
    Simulation(Model model, double endTime, double deltaT, int videoDuration, int fps, std::string in, std::string out);

    void run();

    void plotParticles(int iteration);

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Simulation &simulation);

#endif //PSEMOLDYN_GROUPF_SIMULATION_H
