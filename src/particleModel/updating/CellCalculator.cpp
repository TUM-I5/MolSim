#include "CellCalculator.h"

CellCalculator::CellCalculator(CellContainer &cellContainer, const double delta_t, const std::string& forceType)
    : cellContainer(cellContainer), cell_size(cellContainer.getCellSize()),
    delta_t(delta_t), domain_max_dim(cellContainer.getDomain_Max()),
    particles(*cellContainer.getParticles()){
    if (forceType == "LennJones") {
        // preliminary hardcoded
        double sigma{1.0};
        double epsilon{5.0};
        forceLambda = forceLennJonesPotentialFunction(sigma, epsilon);

    } else if (forceType == "simple") {
        forceLambda = forceSimpleGravitational();

    } else {
        throw std::runtime_error("Provided forceType is invalid: " + forceType);
    }
}

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
    static std::array<dim_t, 3> current;
    static std::array<dim_t, 3> pattern;
    std::array<double, 3> F_ij{};
    //write new path inside current/start and pattern
    cellContainer.setNextPath(current, pattern);

    while(current[0] != dim_t_res) {
        std::vector<Particle*> &cell_1 = particles[current[0]][current[1]][current[2]];
        current[0] += pattern[0];
        current[1] += pattern[1];
        current[2] += pattern[2];

        while(0 < current[0] && 0 < current[1] && 0 < current[2] && current[0] <= domain_max_dim[0]
            && current[1] <= domain_max_dim[1] && current[2] <= domain_max_dim[2]) {

            std::vector<Particle*> &cell_2 = particles[current[0]][current[1]][current[2]];

            for(Particle* p_i : cell_1) {
                for(Particle* p_j : cell_2) {

                    F_ij = forceLambda(*p_i, *p_j);

                    for (int i = 0; i < 3; i++) {
                        p_i->addF(i, F_ij[i]);
                        p_j->addF(i, -F_ij[i]);
                    }
                }
            }

            cell_1 = particles[current[0]][current[1]][current[2]];
            current[0] += pattern[0];
            current[1] += pattern[1];
            current[2] += pattern[2];
        }

        cellContainer.setNextPath(current, pattern);
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
    std::array<double, 3> F_ij{};

    for (auto it1 = current_cell.begin(); it1 != current_cell.end(); ++it1) {
        for(auto it2 = std::next(it1); it2 != current_cell.end(); ++it2) {
            p_i = *it1;
            p_j = *it2;

            F_ij = forceLambda(*p_i, *p_j);

            for (int i = 0; i < 3; i++) {
                p_i->addF(i, F_ij[i]);
                p_j->addF(i, -F_ij[i]);
            }
        }
    }
}


//Top and Bottom
void CellCalculator::calculateBoundariesTopOrBottom(dim_t z_plane, dim_t z_border){
    dim_t x, y;
    x = y =  1;

  // bottom boundary
  while (y < domain_max_dim[1]) {
    std::vector<Particle*>& cell = particles[x][y][z_plane];

    for (auto particle_pointer : cell) {
      Particle& particle = *particle_pointer;
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];
      // a assume that we have an offset of 1 everywhere
      double distance =z_dim - z_border;

      if (std::abs(distance) < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_z = z_dim - 2*distance;

        std::array<double,3> F_particle_ghost = force(particle,{x_dim,y_dim,ghost_particle_z});
        particle.addF(0, F_particle_ghost[0]);
        particle.addF(1, F_particle_ghost[1]);
        particle.addF(2, F_particle_ghost[2]);
      }
    }

    x++;
    if (x >= domain_max_dim[0]) {
      x = 1;
      y++;
    }
  }

};

//Front and Back
void CellCalculator::calculateBoundariesFrontOrBack(dim_t x_plane,dim_t x_border, dim_t z_until){
    dim_t y, z;
    z = y =  1;

  // bottom boundary
  while (z < z_until) {
    std::vector<Particle*>& cell = particles[x_plane][y][z];

    for (auto particle_pointer : cell) {
      Particle& particle = *particle_pointer;
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];
      // a assume that we have an offset of 1 everywhere
        double distance = x_dim - x_border;

      if (std::abs(distance) < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_x = x_dim - 2 * distance;

        std::array<double,3> F_particle_ghost = force(particle,{ghost_particle_x,y_dim,z_dim});
        particle.addF(0, F_particle_ghost[0]);
        particle.addF(1, F_particle_ghost[1]);
        particle.addF(2, F_particle_ghost[2]);
      }
    }

    y++;
    if (y >= domain_max_dim[1]) {
      y = 1;
      z++;
    }
  }


}; //Front and Back

//Left and Right
void CellCalculator::calculateBoundariesLeftOrRight(dim_t y_plane,dim_t y_border, dim_t z_until){
    dim_t x, z;
    z = x =  1;

  // bottom boundary
  while (z < z_until) {
    std::vector<Particle*>& cell = particles[x][y_plane][z];

    for (auto particle_pointer : cell) {
      Particle& particle = *particle_pointer;
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];
      // a assume that we have an offset of 1 everywhere
        double distance = y_dim - y_border;

      if (std::abs(distance) < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_y = y_dim - 2 * distance;

        std::array<double,3> F_particle_ghost = force(particle,{x_dim,ghost_particle_y,z_dim});
        particle.addF(0, F_particle_ghost[0]);
        particle.addF(1, F_particle_ghost[1]);
        particle.addF(2, F_particle_ghost[2]);
      }
    }

    x++;
    if (x >= domain_max_dim[0]) {
      x = 1;
      z++;
    }
  }
}; //Left and Right


/**
 * 
 * @brief calculates reflecting boundary conditions by applying Ghost Particles
 * 
 * This method assumes, that at the moment it is called all particles are within the 
 * domain boundaries. It a particle has coordinates outside the domain it is undefined behaviour.
 * 
 * 
 * 
 * 
*/
void CellCalculator::applyGhostParticles() {
  auto domain_border = cellContainer.getDomainBounds();
  dim_t  z_max =  cellContainer.getThreeDimensions()?  domain_max_dim[2] : 1;
  calculateBoundariesTopOrBottom(1,0); //Bottom
  calculateBoundariesTopOrBottom(domain_max_dim[2],domain_border[2]); //Top
  calculateBoundariesFrontOrBack(1,0,z_max); //Front
  calculateBoundariesFrontOrBack(domain_max_dim[0],domain_border[0],z_max); //Back
  calculateBoundariesLeftOrRight(1,0,z_max); //Left
  calculateBoundariesLeftOrRight(domain_max_dim[1],domain_border[1],z_max); //Right
}
