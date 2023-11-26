#include "CellContainer.h"
#include <cmath>

CellContainer::CellContainer(size_t domain_x, size_t domain_y, size_t domain_z, double r_cutoff, double cell_size)
        : particles(domain_x + 2,
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
    std::array<size_t, 3> pos;
    allocateCell(x_arg, pos);
    particles.at(pos[0]).at(pos[1]).at(pos[2]).emplace(x_arg, v_arg, m_arg);
}

void CellContainer::allocateCell(std::array<double, 3> x, std::array<size_t, 3> cell_position) {

}

void CellContainer::plotParticles(outputWriter::VTKWriter &writer) {

}

size_t CellContainer::size() {
    return particle_amount;
}





