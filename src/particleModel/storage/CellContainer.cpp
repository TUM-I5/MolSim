#include "CellContainer.h"
#include <cmath>


CellContainer::CellContainer(size_t domain_x, size_t domain_y, size_t domain_z, double r_cutoff, double cell_size)
        : cell_size(cell_size), domain_dim({domain_x, domain_y, domain_z}),
          domain_borders({domain_x * cell_size, domain_y * cell_size, domain_z * cell_size}),
          particles(domain_x + 2,
                    std::vector<std::vector<std::unordered_set<Particle, ParticleHashing>>>(
                            domain_y + 2,
                            std::vector<std::unordered_set<Particle, ParticleHashing>>(
                                    domain_z + 2
                                    ))){
    if (cell_size < r_cutoff) {
        comparing_depth = std::ceil(r_cutoff / cell_size);
    }
}

CellContainer::~CellContainer() {}

void CellContainer::addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg) {
    if(domain_borders[0] < x_arg[0] || domain_borders[1] < x_arg[1] || domain_borders[2] < x_arg[2]) {
        throw std::invalid_argument("The provided coordinates are outside the domain borders.");
    }

    static std::array<unsigned short, 3> pos;
    allocateCell(x_arg, pos);
    particles.at(pos[0]).at(pos[1]).at(pos[2]).emplace(x_arg, v_arg, m_arg);
    particle_amount++;
}

void CellContainer::allocateCell(std::array<double, 3> &x, std::array<unsigned short, 3> &cell_position) {
    cell_position[0] = static_cast<unsigned short>(x[0] / cell_size + 1);
    cell_position[1] = static_cast<unsigned short>(x[1] / cell_size + 1);
    cell_position[2] = static_cast<unsigned short>(x[2] / cell_size + 1);
}

void CellContainer::plotParticles(outputWriter::VTKWriter &writer) {

}

std::string CellContainer::to_string() {
    return "";
}

size_t CellContainer::size() {
    return particle_amount;
}






