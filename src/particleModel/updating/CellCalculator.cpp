#include "CellCalculator.h"

// todo check if particles not copied
CellCalculator::CellCalculator(CellContainer &cellContainer,
                               const double delta_t, ForceCalculation force)
    : Calculator(delta_t, force),
      cellContainer(cellContainer),
      particles(*cellContainer.getParticles()) {}

void CellCalculator::initalize() { (*this).initializeFX(); }

void CellCalculator::iteration() {
  (*this).calculateLinkedCellF();
  (*this).calculateWithinFVX();
}

// todo test and compare with old methods, that calculations remain the same
// todo update cells when particles move
void CellCalculator::initializeFX() {
  static std::array<dim_t, 3> current_position;

  // calculate F
  calculateLinkedCellF();

  // write new coordinates in current_position array
  cellContainer.setNextCell(current_position);

  while (current_position[0] != dim_t_res) {
    std::vector<Particle> &current_cell =
        particles[current_position[0]][current_position[1]]
                 [current_position[2]];

    // finish F calculation
    // calculates forces between the particles in the current container
    for (auto it1 = current_cell.begin(); it1 != current_cell.end(); ++it1) {
      for (auto it2 = std::next(it1); it2 != current_cell.end(); ++it2) {
        std::array<double, 3> F_ij = forceLambda(*it1, *it2);

        for (int i = 0; i < 3; i++) {
          it1->addF(i, F_ij[i]);
          it2->addF(i, -F_ij[i]);
        }
      }
    }

    for (Particle &particle : current_cell) {
      const std::array<double, 3> &f = particle.getF();
      const std::array<double, 3> &x = particle.getX();
      const std::array<double, 3> &v = particle.getV();
      const double &m = particle.getM();

      // calculate X
      for (int i = 0; i < 3; i++) {
        particle.setX(
            i, x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);
      }

      // shift F
      particle.shiftF();
    }

    cellContainer.setNextCell(current_position);
  }
}

void CellCalculator::calculateLinkedCellF() {
  static std::array<dim_t, 3> start;
  static std::array<dim_t, 3> pattern;
  // write new path inside start and pattern
  cellContainer.setNextPath(start, pattern);

  while (start[0] != dim_t_res) {
    // todo while not outside bounds
    while (start[0] != 0 && start[1] != 0 && start[2] != 0) {
      // combine and calcualte force between vectors

      start[0] += pattern[0];
      start[1] += pattern[1];
      start[2] += pattern[2];
    }

    cellContainer.setNextPath(start, pattern);
  }
}

void CellCalculator::calculateWithinFVX() {
  static std::array<dim_t, 3> current_position;
  // write new coordinates in current_position array
  cellContainer.setNextCell(current_position);

  while (current_position[0] != dim_t_res) {
    std::vector<Particle> &current_cell =
        particles[current_position[0]][current_position[1]]
                 [current_position[2]];

    // finish F calculation
    for (auto it1 = current_cell.begin(); it1 != current_cell.end(); ++it1) {
      for (auto it2 = std::next(it1); it2 != current_cell.end(); ++it2) {
        // todo apply force calcualtion on it1 and it2
        std::array<double, 3> F_ij = forceLambda(*it1, *it2);

        for (int i = 0; i < 3; i++) {
          it1->addF(i, F_ij[i]);
          it2->addF(i, -F_ij[i]);
        }
      }
    }

    calculateBoundaries();

    for (Particle &particle : current_cell) {
      const std::array<double, 3> &f = particle.getF();
      const std::array<double, 3> &f_old = particle.getOldF();
      const std::array<double, 3> &x = particle.getX();
      const std::array<double, 3> &v = particle.getV();
      const double &m = particle.getM();

      // calculate V
      for (int i = 0; i < 3; i++) {
        particle.setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
      }

      // calculate X
      for (int i = 0; i < 3; i++) {
        particle.setX(
            i, x[i] + delta_t * v[i] + delta_t * delta_t * f[i] / 2.0 / m);

        // updates the cell of the particle if required
        cellContainer.updateParticleCell(particle, current_position);
      }

      // shift F
      particle.shiftF();
    }

    cellContainer.setNextCell(current_position);
  }
}

void CellCalculator::calculateBoundariesHorizontal(dim_t z,bool add, ForceCalculation_Ghost force){
    dim_t x, y;
    x = y =  1;
    auto domain_max = cellContainer.domain_max;
  double ref_size = std::pow(2, 1.0 / 6);

  // bottom boundary
  while (y < domain_max[1]) {
    std::vector<Particle> cell = particles[x][y][z];

    for (auto &particle : cell) {
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];
      // a assume that we have an offset of 1 everywhere
        double distance =std::abs(z_dim - z);

      if (distance < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_z = z_dim + 2*(add ? distance : -distance);

        std::array<double,3> F_particle_ghost = force(particle,{x_dim,y_dim,ghost_particle_z});
        particle.addF(0, F_particle_ghost[0]);
        particle.addF(1, F_particle_ghost[1]);
        particle.addF(2, F_particle_ghost[2]);
      }
    }

    x++;
    if (x >= domain_max[0]) {
      x = 1;
      y++;
    }
  }

};


void CellCalculator::calculateBoundariesVertical(dim_t x,bool add, ForceCalculation_Ghost force){
    dim_t y, z;
    z = y =  1;
    auto domain_max = cellContainer.domain_max;
    double ref_size = std::pow(2, 1.0 / 6);

  // bottom boundary
  while (y < domain_max[1]) {
    std::vector<Particle> cell = particles[x][y][z];

    for (auto &particle : cell) {
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];
      // a assume that we have an offset of 1 everywhere
        double distance =std::abs(x_dim - x);

      if (distance < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_x = x_dim + 2*(add ? distance : -distance);

        std::array<double,3> F_particle_ghost = force(particle,{x_dim,y_dim,ghost_particle_x});
        particle.addF(0, F_particle_ghost[0]);
        particle.addF(1, F_particle_ghost[1]);
        particle.addF(2, F_particle_ghost[2]);
      }
    }

    z++;
    if (z >= domain_max[2]) {
      z = 1;
      y++;
    }
  }


}; //Front and Back


void calculateBoundariesVertical2(dim_t y,bool add, ForceCalculation_Ghost force){}; //Left and Right


void CellCalculator::calculateBoundaries() {


  // top boundary

  // for(auto iter = cellContainer.begin_boundary(); iter !=
  // cellContainer.end_boundary(); ++iter){
  //     //yields one of the boundary cells
  //     std::vector<Particle> cell = *iter;

  // }
}
