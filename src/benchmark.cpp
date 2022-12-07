//
// Created by alex on 26.11.2022.
//

#include "sim/Simulation.h"
#include "benchmark.h"
#include "io/input/cli/CLIArgsParser.h"
#include "data/Body.h"
#include "data/Particle.h"
#include "data/ParticleGenerator.h"

static int runBenchmarkFile(Configuration& config, std::vector<std::string>& files, io::input::type t);

static int runBenchmarkDefault(Configuration& config, io::input::type t);

int runBenchmark(Configuration& config, std::vector<std::string>& files, io::input::type t) {
    if (config.get<benchmarkType>() == "default") return runBenchmarkDefault(config, t);
    if (config.get<benchmarkType>() == "file") return runBenchmarkFile(config, files, t);
    else io::input::exitFormatError(config.get<benchmarkType>() + ": is an unknown benchmark input type!");
}

static int runBenchmarkDefault(Configuration& config, io::input::type t) {
    int maxBodySize = config.get<benchMaxBodySize>();

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
        io::IOWrapper iow {t};
        sim::Simulation simulation {iow, pc, config};
        simulation.runBenchmark(config.get<benchIterationCount>(), "default", buffer,
                                config.get<boundingBox_X0>(),
                                config.get<boundingBox_X1>(),
                                config.get<boundingBox_X2>(),
                                config.get<rCutoff>());

        buffer_tmp.clear();
        buffer.clear();
    }
    return 0;
}

static int
runBenchmarkFile(Configuration& config, std::vector<std::string>& files, io::input::type t) {
    std::vector<Particle> buffer;

    for (const auto &file: files) {
        Configuration configActive = config;
        auto iow = io::IOWrapper(t, file);
        iow.reload();
        iow.getParticles(buffer);
        configActive.loadIOWArgs(iow.getArgMap());

        ParticleContainer pc {};
        sim::Simulation simulation {iow, pc, configActive};
        simulation.runBenchmark(configActive.get<io::input::benchIterationCount>(), file, buffer,
                                config.get<boundingBox_X0>(),
                                config.get<boundingBox_X1>(),
                                config.get<boundingBox_X2>(),
                                config.get<rCutoff>());
        buffer.clear();
    }
    return 0;
}