#include "CellCalculator.h"
#include "particleModel/storage/CellContainerIterators.h"
#include "utils/ArrayUtils.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <limits>

double min_distance = 0.9;

std::vector<std::vector<double>> sigma_mixed{{1.0}};

std::vector<std::vector<double>> epsilon_mixed{{5.0}};

CellCalculator::CellCalculator(CellContainer &cellContainer, double delta_t,
      const std::string& forceType, std::array<boundary_conditions,6> boundaries_cond,
      double gravity_factor, double target_temp_param, double max_temp_diff_param)
    : cellContainer(cellContainer), cell_size(cellContainer.getCellSize()),
    gravity_factor(gravity_factor), delta_t(delta_t), domain_max_dim(cellContainer.getDomain_Max()),
    domain_bounds(cellContainer.getDomainBounds()), max_temp_diff(max_temp_diff_param),  target_temp(target_temp_param),
    boundaries(boundaries_cond), particles(*cellContainer.getParticles()){

    ref_size = std::pow(2, 1.0 / 6);

    if(boundaries[0] == boundary_conditions::periodic && (
            boundaries[1] != boundary_conditions::periodic ||
            boundaries[2] != boundary_conditions::periodic ||
            boundaries[3] != boundary_conditions::periodic ||
            boundaries[4] != boundary_conditions::periodic ||
            boundaries[5] != boundary_conditions::periodic
            )) {
        throw std::runtime_error("Provided boundary conditions are invalid: either all should be periodic or none.");
    }
}

void CellCalculator::initializeFX() {
    std::array<dim_t, 3> current_position;
    instructions cell_updates;

    //calculate F
    calculateLinkedCellF();

    //write new coordinates in current_position array
    cellContainer.setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle*> *current_cell = &particles[current_position[0]][current_position[1]][current_position[2]];

        //finish F calculation
        finishF(current_cell);

        for (auto iter = current_cell->begin(); iter != current_cell->end();) {
            Particle& particle = *(*iter);

            calculateVX(particle, false);
            particle.shiftF();

            std::array<dim_t, 3> position;
            cellContainer.allocateCell(particle.getX(),position);
            
            if (position[0] != current_position[0] ||
                position[1] != current_position[1] ||
                position[2] != current_position[2])
            {
                if(boundaries[0] == boundary_conditions::periodic) {
                    std::array<double,3> particle_offset{0,0,0};

                    //mirror new cell position into the domain cells
                    mirror(position, particle_offset);
                    //update particle position for mirrored cell
                    (*iter)->addX(particle_offset);
                }

                cell_updates.emplace_back(*iter,position);
                iter = current_cell->erase(iter);

            }else{
                iter++;
            }
        }

        cellContainer.setNextCell(current_position);
    }

    //update particle distribution in the cells
    updateCells(cell_updates);
}


void CellCalculator::calculateLinkedCellF() {
    static std::array<dim_t, 3> current;
    static std::array<dim_t, 3> pattern;
    std::vector<Particle*>* cell_1;
    std::vector<Particle*>* cell_2;
    std::array<double, 3> F_ij{};

    //write new path inside current/start and pattern
    cellContainer.setNextPath(current, pattern);

    while(current[0] != dim_t_res) {
        cell_1 = &particles[current[0]][current[1]][current[2]];
        current[0] += pattern[0];
        current[1] += pattern[1];
        current[2] += pattern[2];

        while(0 < current[0] && 0 < current[1] && 0 < current[2] && current[0] <= domain_max_dim[0]
              && current[1] <= domain_max_dim[1] && current[2] <= domain_max_dim[2]) {

            cell_2 = &particles[current[0]][current[1]][current[2]];

            for(auto & p_i : *cell_1) {
                for(auto & p_j : *cell_2) {

                    F_ij = force(*p_i, *p_j, {0,0,0});

                    for (int i = 0; i < 3; i++) {
                        p_i->addF(i, F_ij[i]);
                        p_j->addF(i, -F_ij[i]);
                    }
                }
            }

            cell_1 = &particles[current[0]][current[1]][current[2]];
            current[0] += pattern[0];
            current[1] += pattern[1];
            current[2] += pattern[2];
        }

        if(boundaries[0] == boundary_conditions::periodic) {
            std::array<double,3> particle_offset{0,0,0};

            //mirror the last position into the domain
            mirror(current, particle_offset);

            cell_2 = &particles[current[0]][current[1]][current[2]];

            for(auto & p_i : *cell_1) {
                for(auto & p_j : *cell_2) {

                    F_ij = force(*p_i, *p_j, particle_offset);

                    for (int i = 0; i < 3; i++) {
                        p_i->addF(i, F_ij[i]);
                        p_j->addF(i, -F_ij[i]);
                    }
                }
            }
        }

        cellContainer.setNextPath(current, pattern);
    }
}


void CellCalculator::calculateWithinFVX() {
    std::array<dim_t, 3> current_position;
    instructions cell_updates;

    //write new coordinates in current_position array
    cellContainer.setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle*> *current_cell = &particles[current_position[0]][current_position[1]][current_position[2]];
        //finish F calculation
        finishF(current_cell);

        for (auto iter = current_cell->begin(); iter != current_cell->end();) {
            Particle* particle_ptr = *iter;
            Particle& particle = *particle_ptr;

            calculateVX(particle, true);
            particle.shiftF();

            std::array<dim_t, 3> position;
            cellContainer.allocateCell(particle.getX(),position);

            if (position[0] != current_position[0] ||
                position[1] != current_position[1] ||
                position[2] != current_position[2])
            {
                if(boundaries[0] == boundary_conditions::periodic) {
                    std::array<double,3> particle_offset{0,0,0};

                    //mirror new cell position into the domain cells
                    mirror(position, particle_offset);
                    //update particle position for mirrored cell
                    (*iter)->addX(particle_offset);
                }

                cell_updates.emplace_back(*iter,position);
                iter = current_cell->erase(iter);
            } else{
                iter++;
            }
        }
        cellContainer.setNextCell(current_position);
    }
    updateCells(cell_updates);
}

void CellCalculator::updateCells(instructions& cell_updates) {
  static int amt_removed = 0;
    for(auto ins : cell_updates){

      Particle* particle_ptr = std::get<0>(ins);
      std::array<dim_t, 3> new_cell_position = std::get<1>(ins);

      if( 0 < new_cell_position[0] &&  new_cell_position[0] <= domain_max_dim[0] &&
            0 < new_cell_position[1] &&  new_cell_position[1] <= domain_max_dim[1] &&
            0 < new_cell_position[2] && new_cell_position[2] <= domain_max_dim[2]) {

          std::vector<Particle*> *new_cell = &particles[new_cell_position[0]][new_cell_position[1]][new_cell_position[2]];
          new_cell->push_back(particle_ptr);
      }
      else{
          //std::vector<Particle*> *new_cell = &particles[new_cell_position[0]][new_cell_position[1]][new_cell_position[2]];
          //new_cell->push_back(particle_ptr);

          SPDLOG_INFO("new halo particle: " + (*particle_ptr).toString());
          cellContainer.getHaloParticles().push_back(particle_ptr);
          amt_removed++;
      }
    }
}


void CellCalculator::mirror(std::array<dim_t,3> &position, std::array<double,3> &offset) {
    //move on the other side in x
    if(position[0] < 1) {
        position[0] = position[0] + domain_max_dim[0];
        offset[0] = domain_bounds[0];

    } else if(domain_max_dim[0] < position[0]) {
        position[0] = position[0] - domain_max_dim[0];
        offset[0] = -domain_bounds[0];
    }
    //move on the other side in y
    if(position[1] < 1) {
        position[1] = position[1] + domain_max_dim[1];
        offset[1] = domain_bounds[1];

    } else if(domain_max_dim[1] < position[1]) {
        position[1] = position[1] - domain_max_dim[1];
        offset[1] = -domain_bounds[1];
    }
    //move on the other side in z
    if(position[2] < 1) {
        position[2] = position[2] + domain_max_dim[2];
        offset[2] = domain_bounds[2];

    } else if(domain_max_dim[2] < position[2]) {
        position[2] = position[2] - domain_max_dim[2];
        offset[2] = -domain_bounds[2];
    }
}


void CellCalculator::calculateVX(Particle &particle, bool calculateV) {
    const std::array<double, 3> &f = particle.getF();
    const std::array<double, 3> &x = particle.getX();
    const std::array<double, 3> &v = particle.getV();
    const double &m = particle.getM();

    if(calculateV) {
        const std::array<double, 3> &f_old = particle.getOldF();


        double kinetic_en_particle = 0;
        //calculate V
        for (int i = 0; i < 3; i++) {
            particle.setV(i, v[i] + delta_t * (f[i] + f_old[i]) / (2 * m));
        }
    }

    //calculate X
    double x_0 = x[0] + delta_t * v[0] + delta_t * delta_t * f[0] / 2.0 / m;
    double x_1 = x[1] + delta_t * v[1] + delta_t * delta_t * f[1] / 2.0 / m;
    double x_2 = x[2] + delta_t * v[2] + delta_t * delta_t * f[2] / 2.0 / m;

    /* reflection fix:
    double offset = 0.001;
    if(boundaries[0] == boundary_conditions::reflective && domain_bounds[2] < x_2) {
        x_2 = domain_bounds[2] - offset;
        particle.addF(2,-f[2]);
    }
    if(boundaries[1] == boundary_conditions::reflective && x_2 < 0) {
        x_2 = offset;
        particle.addF(2,-2 * f[2]);
    }
    if(boundaries[2] == boundary_conditions::reflective && domain_bounds[0] < x_0) {
        x_0 = domain_bounds[0] - offset;
        particle.addF(0,-2 * f[0]);
    }
    if(boundaries[3] == boundary_conditions::reflective && x_0 < 0) {
        x_0 = offset;
        particle.addF(0,-2 * f[0]);
    }
    if(boundaries[4] == boundary_conditions::reflective && domain_bounds[1]  < x_1) {
        x_1 = domain_bounds[1] - offset;
        particle.addF(1,-2 * f[1]);
    }
    if(boundaries[5] == boundary_conditions::reflective && x_1 < 0) {
        x_1 = offset;
        particle.addF(1,-2 * f[1]);
    }
     */


    particle.setX(0, x_0);
    particle.setX(1, x_1);
    particle.setX(2, x_2);
}


void CellCalculator::applyThermostats(){
  double kinetic_energy = 0;
  size_t amt = 0;
  for(auto iter = cellContainer.begin(); iter != cellContainer.end(); ++iter){
    std::vector<Particle*> cell = particles[iter.x][iter.y][iter.z];
    for(Particle* particle_ptr : cell){
      std::array<double,3> v = particle_ptr->getV();
      double v_squared = v[0] * v[0]  + v[1] * v[1] + v[2] * v[2];
      double m = particle_ptr->getM();
      //std::cout << "v_squared: " << v_squared << " m: " << m << " \n";
      kinetic_energy += v_squared * m;
      amt++;
    }
  }
  double k_boltzman = 1;
  //std::cout << "kinetic: " << kinetic_energy << "\n";
  //calculate the current temperatur from the current kinetic energy in the system
  //assuming we only have two kinds of dimensions namely 2 or 3
  double current_temp = kinetic_energy/((cellContainer.hasThreeDimensions() ? 3 : 2) * amt * k_boltzman);
   //std::cout << "previous temp: " << current_temp << std::endl;
  double next_temp = target_temp;
  //std::cout << "adjusting to " << next_temp << "\n\n" << std::endl;

  //if the temperatur diffference would be too big cap it
  double temp_diff = target_temp - current_temp;
  if(std::abs(temp_diff) > max_temp_diff){
    //std::cout << "Capping Temperature diff\n";
    next_temp = (std::signbit(temp_diff) ? -1 : 1) * max_temp_diff + current_temp;
  }


  // the scaling factor to reach the target temperature
  // assuming this is never negative because we only calculate with kelvin
  double temp_scaling = sqrt(next_temp/current_temp);

  //apply scaling
  for(auto iter = cellContainer.begin(); iter != cellContainer.end(); ++iter){
      std::vector<Particle*> cell = particles[iter.x][iter.y][iter.z];
      for(Particle* particle_ptr : cell){
        std::array<double,3> v = particle_ptr->getV();
        for(int i = 0; i < 3; i++){
          particle_ptr->setV(i,temp_scaling*v[i]);
        }
      }
  }
}


void CellCalculator::finishF(std::vector<Particle*> *current_cell) {
    Particle* p_i;
    Particle* p_j;
    std::array<double, 3> F_ij{};

    for (auto it1 = current_cell->begin(); it1 != current_cell->end(); ++it1) {
        for(auto it2 = std::next(it1); it2 != current_cell->end(); ++it2) {
            p_i = *it1;
            p_j = *it2;

            F_ij = force(*p_i, *p_j, {0,0,0});

            for (int i = 0; i < 3; i++) {
                p_i->addF(i, F_ij[i]);
                p_j->addF(i, -F_ij[i]);
            }
        }

        //add gravity
        (*it1)->addF(1, (*it1)->getM() * gravity_factor);
    }
}

std::array<double,3> CellCalculator::force(const Particle &p_i, const Particle &p_j, const std::array<double,3> &offset) {
    auto x_i = p_i.getX(), x_j = p_j.getX();

    double sigma = sigma_mixed[p_i.getType()][p_j.getType()];
    double epsilon = epsilon_mixed[p_i.getType()][p_j.getType()];

    double norm = ArrayUtils::L2Norm(x_i - x_j + offset);
    norm = std::max(min_distance, norm);

    double prefactor = (-24 * epsilon) / (std::pow(norm, 2));

    prefactor *= (std::pow(sigma / norm, 6) - 2 * std::pow(sigma / norm, 12));

    return prefactor * (x_i - x_j + offset);
}


void CellCalculator::calculateBoundariesTopOrBottom(dim_t lower_z,dim_t upper_z, double z_border){
    dim_t x, y;
    x = y =  1;
    Particle dummy{{0,0,0},{0,0,0},1};

  // bottom boundary
  while(lower_z <= upper_z){
  while (y < domain_max_dim[1]) {
    std::vector<Particle*>& cell = particles[x][y][lower_z];

    for (auto particle_pointer : cell) {
      Particle& particle = *particle_pointer;
      double x_dim = particle.getX()[0];
      double y_dim = particle.getX()[1];
      double z_dim = particle.getX()[2];

      // a assume that we have an offset of 1 everywhere
      double distance = z_dim - z_border;

      if (std::abs(distance) < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_z = z_dim - 2*distance;

        std::array<double,3> F_particle_ghost = force(particle, dummy, {-x_dim,-y_dim,-ghost_particle_z});
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
  x = 1;
  y = 1;
  lower_z++;
  }

};


void CellCalculator::calculateBoundariesFrontOrBack(dim_t lower_x,dim_t upper_x ,double x_border, dim_t z_until){
    dim_t y, z;
    z = y =  1;
    Particle dummy{{0,0,0},{0,0,0},1};

  // bottom boundary
  while(lower_x <= upper_x){
  while (z <= z_until) {
    std::vector<Particle*>& cell = particles[lower_x][y][z];

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

        std::array<double,3> F_particle_ghost = force(particle,dummy,{-ghost_particle_x,-y_dim,-z_dim});
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
  y = 1;
  z = 1;
  lower_x++;
  }
}; //Front and Back



void CellCalculator::calculateBoundariesLeftOrRight(dim_t lower_y,dim_t upper_y ,double y_border, dim_t z_until){
    dim_t x, z;
    z = x =  1;
    Particle dummy{{0,0,0},{0,0,0},1};

  // bottom boundary
  while(lower_y <= upper_y){
  while (z <= z_until) {
    std::vector<Particle*>& cell = particles[x][lower_y][z];

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

        std::array<double,3> F_particle_ghost = force(particle,dummy,{-x_dim,-ghost_particle_y,-z_dim});
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
  x = 1;
  z = 1;
  lower_y++;
  }
}; //Left and Right



void CellCalculator::applyBoundaries() {
  dim_t  z_max = cellContainer.hasThreeDimensions() ? domain_max_dim[2] : 1;
  dim_t comparing_depth = cellContainer.getComparingdepth();

  if(cellContainer.hasThreeDimensions()){

      //TOP SIDE
      //boundaries[0] corresponds to boundary_conditions in positiveZ direction
      if(boundaries[0] == boundary_conditions::reflective)
        calculateBoundariesTopOrBottom(domain_max_dim[2]-comparing_depth,domain_max_dim[2],domain_bounds[2]);

      //BOTTOM SIDE
      //boundaries[1] corresponds to boundary_conditions in negativeZ direction
      if(boundaries[1] == boundary_conditions::reflective)
        calculateBoundariesTopOrBottom(1,comparing_depth,0); 
  }


  //BACK SIDE
  //boundaries[2] corresponds to boundary_conditions in positiveX direction
  if(boundaries[2] == boundary_conditions::reflective){
    calculateBoundariesFrontOrBack(domain_max_dim[0]-comparing_depth,domain_max_dim[0],domain_bounds[0],z_max);
    
  }


  //FRONT SIDE
  //boundaries[3] corresponds to boundary_conditions in negativeX direction
  if(boundaries[3] == boundary_conditions::reflective){
    calculateBoundariesFrontOrBack(1,comparing_depth+1,0,z_max); 
    
  }


  //RIGHT SIDE
  //boundaries[4] corresponds to boundary_conditions in positiveY direction
  if(boundaries[4] == boundary_conditions::reflective){
    calculateBoundariesLeftOrRight(domain_max_dim[1]-comparing_depth,domain_max_dim[1],domain_bounds[1],z_max); //Right
    
  }


  //LEFT SIDE
  //boundaries[5] corresponds to boundary_conditions in negativeY direction
  if(boundaries[5] == boundary_conditions::reflective) {
    calculateBoundariesLeftOrRight(1,comparing_depth+1,0,z_max); 
    
  }
}
