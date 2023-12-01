#include "CellCalculator.h"

CellCalculator::CellCalculator(CellContainer &cellContainer, const double delta_t)
    : cellContainer(cellContainer), cell_size(cellContainer.getCellSize()),
    delta_t(delta_t), particles(*cellContainer.getParticles()){}

//todo test and compare with old methods, that calculations remain the same
void CellCalculator::initializeFX() {
    static std::array<dim_t, 3> current_position;
    static instructions cell_updates;

    //calculate F
    calculateLinkedCellF();

    //write new coordinates in current_position array
    cellContainer.setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle*> &current_cell = particles[current_position[0]][current_position[1]][current_position[2]];

        //finish F calculation
        finishF(current_cell);

        size_t particle_index = 0;
        for (Particle* p : current_cell) {
            Particle &particle = *p;

            calculateVX(particle, current_position, cell_updates, false, particle_index);
            ++particle_index;

            //shift F
            particle.shiftF();
        }

        cellContainer.setNextCell(current_position);
    }

    //update particle distribution in the cells
    updateCells(cell_updates);
}

void CellCalculator::calculateLinkedCellF() {
    static std::array<dim_t, 3> start;
    static std::array<dim_t, 3> pattern;
    //write new path inside start and pattern
    cellContainer.setNextPath(start, pattern);

    while(start[0] != dim_t_res) {
        //todo while not outside bounds
        while(start[0] != 0 && start[1] != 0 && start[2] != 0) {

            //combine and calcualte force between vectors

            start[0] += pattern[0];
            start[1] += pattern[1];
            start[2] += pattern[2];
        }

        cellContainer.setNextPath(start, pattern);
    }
}

void CellCalculator::calculateWithinFVX() {
    static std::array<dim_t, 3> current_position;
    static instructions cell_updates;

    //write new coordinates in current_position array
    cellContainer.setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle*> &current_cell = particles[current_position[0]][current_position[1]][current_position[2]];

        //finish F calculation
        finishF(current_cell);

        size_t particle_index = 0;
        for (Particle* p : current_cell) {
            Particle &particle = *p;

            calculateVX(particle, current_position, cell_updates, true, particle_index);
            ++particle_index;

            //shift F
            particle.shiftF();
        }

        cellContainer.setNextCell(current_position);
    }

    //update particle distribution in the cells
    updateCells(cell_updates);
}

void CellCalculator::updateCells(instructions cell_updates) {
    //todo check for outflow here
    while(!cell_updates.empty()) {
        static std::tuple<size_t , std::array<dim_t,3>,std::array<dim_t,3>> move_instruction = cell_updates.back();
        move_instruction = cell_updates.back();

        size_t particle_index = std::get<0>(move_instruction);
        static std::array<dim_t, 3> old_cell_position = std::get<1>(move_instruction);
        static std::array<dim_t, 3> new_cell_position = std::get<2>(move_instruction);

        std::vector<Particle*> &old_cell = particles[old_cell_position[0]][old_cell_position[1]][old_cell_position[2]];
        std::vector<Particle*> &new_cell = particles[new_cell_position[0]][new_cell_position[1]][new_cell_position[2]];

        Particle* particle = old_cell[particle_index];
        old_cell.erase(old_cell.begin() + static_cast<long>(particle_index));
        new_cell.push_back(particle);

        cell_updates.pop_back();
    }
}

void CellCalculator::calculateVX(Particle &particle, std::array<dim_t, 3> &current_position,
                                 instructions &cell_updates, bool calculateV, size_t particle_index) {
    const std::array<double, 3> &f = particle.getF();
    const std::array<double, 3> &x = particle.getX();
    const std::array<double, 3> &v = particle.getV();
    const double &m = particle.getM();

    if(calculateV) {
        const std::array<double, 3> &f_old = particle.getOldF();

        //calculate V
        for (int i = 0; i < 3; i++) {
            particle.setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
        }
    }


    //calculate X
    double x_0 = x[0] + delta_t * v[0] + delta_t * delta_t * f[0] / 2.0 / m;
    double x_1 = x[1] + delta_t * v[1] + delta_t * delta_t * f[1] / 2.0 / m;
    double x_2 = x[2] + delta_t * v[2] + delta_t * delta_t * f[2] / 2.0 / m;

    particle.setX(0, x_0);
    particle.setX(1, x_1);
    particle.setX(2, x_2);

    //todo catch negative x and movement out of "smaller" cells

    std::array<dim_t, 3> position{static_cast<dim_t>(x_0 / cell_size + 1),
                                  static_cast<dim_t>(x_1 / cell_size + 1),
                                  static_cast<dim_t>(x_2 / cell_size + 1)};

    if(position[0] != current_position[0] ||
       position[1] != current_position[1] ||
       position[2] != current_position[2]) {
        cell_updates.emplace_back(particle_index, current_position, position);
    }
}

void CellCalculator::finishF(std::vector<Particle*> &current_cell) {
    Particle* p_i;
    Particle* p_j;

    for (auto it1 = current_cell.begin(); it1 != current_cell.end(); ++it1) {
        for(auto it2 = std::next(it1); it2 != current_cell.end(); ++it2) {
            p_i = *it1;
            p_j = *it2;

            //todo apply force calcualtion on it1 and it2
            std::array<double, 3> F_ij{0.0, 0.0, 0.0};

            for (int i = 0; i < 3; i++) {
                p_i->addF(i, F_ij[i]);
                p_j->addF(i, -F_ij[i]);
            }
        }
    }
}

void CellCalculator::applyGhostParticles() {
    //todo iterate over domain boundaries and apply forces on particles too close to the bounding box
}
