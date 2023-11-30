#include "CellCalculator.h"

//todo check if particles not copied
CellCalculator::CellCalculator(CellContainer &cellContainer, const double delta_t)
    : cellContainer(cellContainer), delta_t(delta_t), particles(*cellContainer.getParticles()){}

//todo test and compare with old methods, that calculations remain the same
//todo update cells when particles move
void CellCalculator::initializeFX() {
    static std::array<dim_t, 3> current_position;

    //calculate F
    calculateLinkedCellF();

    //write new coordinates in current_position array
    cellContainer.setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle> &current_cell = particles[current_position[0]][current_position[1]][current_position[2]];

        //finish F calculation
        for (auto it1 = current_cell.begin(); it1 != current_cell.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != current_cell.end(); ++it2) {
                //todo apply force calculation on it1 and it2
                std::array<double, 3> F_ij{0.0, 0.0, 0.0};

                for (int i = 0; i < 3; i++) {
                    it1->addF(i, F_ij[i]);
                    it2->addF(i, -F_ij[i]);
                }
            }
        }

        for (Particle& particle : current_cell) {
            const std::array<double, 3> &f = particle.getF();
            const std::array<double, 3> &x = particle.getX();
            const std::array<double, 3> &v = particle.getV();
            const double &m = particle.getM();

            //calculate X
            for (int i = 0; i < 3; i++) {
                particle.setX(i, x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);
            }

            //shift F
            particle.shiftF();
        }

        cellContainer.setNextCell(current_position);
    }
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
    //write new coordinates in current_position array
    cellContainer.setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle> &current_cell = particles[current_position[0]][current_position[1]][current_position[2]];

        //finish F calculation
        for (auto it1 = current_cell.begin(); it1 != current_cell.end(); ++it1) {
            for(auto it2 = std::next(it1); it2 != current_cell.end(); ++it2) {
                //todo apply force calcualtion on it1 and it2
                std::array<double, 3> F_ij{0.0, 0.0, 0.0};

                for (int i = 0; i < 3; i++) {
                    it1->addF(i, F_ij[i]);
                    it2->addF(i, -F_ij[i]);
                }
            }
        }

        for (Particle& particle : current_cell) {
            const std::array<double, 3> &f = particle.getF();
            const std::array<double, 3> &f_old = particle.getOldF();
            const std::array<double, 3> &x = particle.getX();
            const std::array<double, 3> &v = particle.getV();
            const double &m = particle.getM();

            //calculate V
            for (int i = 0; i < 3; i++) {
                particle.setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
            }

            //calculate X
            for (int i = 0; i < 3; i++) {
                particle.setX(i, x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);
            }

            //shift F
            particle.shiftF();
        }

        cellContainer.setNextCell(current_position);
    }
}
