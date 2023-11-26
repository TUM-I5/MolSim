#pragma once

#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include <vector>
#include <unordered_set>


struct ParticleHashing {
    size_t operator()(const Particle& obj) const {

        return std::hash<const void*>()(&obj);
    }
};

class CellContainer {
public:
    CellContainer(size_t domain_x, size_t domain_y, size_t domain_z, double r_cutoff, double cell_size);

    virtual ~CellContainer();

    void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg);

    void allocateCell(std::array<double, 3> x, std::array<size_t, 3> cell_position);

    void plotParticles(outputWriter::VTKWriter &writer);

    size_t size();

private:
    size_t comparing_depth = 1;
    size_t particle_amount = 0;
    std::vector<std::vector<std::vector<std::unordered_set<Particle, ParticleHashing>>>> particles;
};

