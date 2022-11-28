//
// Created by alex on 26.11.2022.
//

#include "benchmark.h"
#include "sim/Simulation.h"
#include "io/input/cli/CLIArgsParser.h"
#include "data/Body.h"
#include "data/Particle.h"
#include "data/ParticleGenerator.h"

static int runBenchmarkFile(
        double dt,
        double et,
        double st,
        double sig,
        double eps,
        std::vector<std::string> &inputFiles,
        int iterations);

static int runBenchmarkDefault(
        double dt,
        double et,
        double st,
        double sig,
        double eps,
        int iterations);

int runBenchmark(double dt, double et, double st, double sig, double eps, std::vector<std::string> &inputFiles) {
    int iterations = std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-i")).value;
    std::string type = std::get<io::input::ArgEntry<std::string>>(io::input::cli_arg_map.at("-bench")).value;

    if (type == "default") return runBenchmarkDefault(dt, et, st, sig, eps, iterations);
    if (type == "file") return runBenchmarkFile(dt, et, st, sig, eps, inputFiles, iterations);
    else io::input::exitFormatError(type + ": is an unknown benchmark input type!");
    return -1;
}

static int runBenchmarkDefault(double dt, double et, double st, double sig, double eps, int iterations) {
    int maxBodySize = std::get<io::input::ArgEntry<int>>(io::input::cli_arg_map.at("-bMax")).value;

    // generate 2 bodies in varying sizes
    struct Body b0{Shape::cuboid, {-1.1, 0, 0}, {10, 10, 1}, 0.1, 0.1, {0.001, 0, 0}};
    struct Body b1{Shape::cuboid, {0.1, 0, 0}, {10, 10, 1}, 0.1, 0.1, {0.001, 0, 0}};
    std::list<Particle> buffer_tmp;
    std::vector<Particle> buffer;
    for (int bSize{1}; bSize <= maxBodySize; bSize *= 2) {
        b0.dimensions[2] = bSize;
        b1.dimensions[2] = bSize;
        ParticleGenerator::generateCuboid(b0, 0.1, buffer_tmp, 3);
        ParticleGenerator::generateCuboid(b1, 0.1, buffer_tmp, 3);

        for (const auto &p: buffer_tmp) buffer.push_back(p);
        ParticleContainer pc {};
        sim::Simulation<calcF, calcX, calcV> simulation{pc, st, et, dt, eps, sig, "", ""};
        simulation.runBenchmark(iterations, "default", buffer);

        buffer_tmp.clear();
        buffer.clear();
    }
    return 0;
}

static int
runBenchmarkFile(double dt, double et, double st, double sig, double eps, std::vector<std::string> &inputFiles,
                 int iterations) {
    std::vector<Particle> buffer;
    for (const auto &file: inputFiles) {
        auto iow = io::IOWrapper<io::input::BodyReader>(file.c_str());
        iow.reload();
        iow.getParticles(buffer);

        bool epsSet = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-eps")).isSet;
        bool sigSet = std::get<io::input::ArgEntry<double>>(io::input::cli_arg_map.at("-sig")).isSet;
        if (!epsSet && iow.getArgMap().contains(io::input::names::epsilon))
            eps = std::stod(iow.getArgMap().at(io::input::names::epsilon));
        if (!sigSet && iow.getArgMap().contains(io::input::names::sigma))
            sig = std::stod(iow.getArgMap().at(io::input::names::sigma));

        ParticleContainer pc {};
        sim::Simulation<calcF, calcX, calcV> simulation{pc, st, et, dt, eps, sig, "", ""};
        simulation.runBenchmark(iterations, file, buffer);
        buffer.clear();
    }
    return 0;
}