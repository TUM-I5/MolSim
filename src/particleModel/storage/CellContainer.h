#pragma once

#include "Particle.h"
#include "outputWriter/VTKWriter.h"
#include <vector>
#include <unordered_set>

typedef unsigned short dim_t;

struct ParticleHashing {
    size_t operator()(const Particle& obj) const {

        return std::hash<const void*>()(&obj);
    }
};

class CellContainer {
public:
    CellContainer(dim_t domain_x, dim_t domain_y, dim_t domain_z, double r_cutoff, double cell_size);

    virtual ~CellContainer();

    void setNextCell(std::array<dim_t, 3> &next_position);

    void setNextPath(std::array<dim_t, 6> &start_and_pattern);

    void addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg);

    void allocateCell(std::array<double, 3> &x, std::array<dim_t , 3> &cell_position);

    void plotParticles(outputWriter::VTKWriter &writer);

    std::string to_string();

    size_t size();

    auto getParticles() {
        return &particles;
    }

private:
    double cell_size;
    std::array<dim_t, 3> domain_dim;
    std::array<double, 3> domain_borders;
    size_t comparing_depth = 1;
    size_t particle_amount = 0;
    std::vector<std::vector<std::vector<std::unordered_set<Particle, ParticleHashing>>>> particles;
};

