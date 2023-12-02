#include <iostream>
#include "CellContainer.h"
#include <cmath>

dim_t dim_t_res = -1;

CellContainer::CellContainer(double d_width, double d_height, double d_depth, double r_cutoff, double cell_size)
            : cell_size(cell_size),
              domain_max_dim({static_cast<dim_t>(d_width / cell_size + 1),
                              static_cast<dim_t>(d_height / cell_size + 1),
                              static_cast<dim_t>(d_depth / cell_size + 1)}),
              domain_bounds({d_width, d_height, d_depth}),
              particles(static_cast<dim_t>(d_width / cell_size + 2),
                    std::vector<std::vector<std::vector<Particle*>>>(
                            static_cast<dim_t>(d_height / cell_size + 2),
                            std::vector<std::vector<Particle*>>(
                                    static_cast<dim_t>(d_depth / cell_size + 2)
                                    ))){
    if (cell_size < r_cutoff) {
        comparing_depth = std::ceil(r_cutoff / cell_size);
    }

    if(domain_max_dim[2] == 1) {

        if(domain_max_dim[0] <= comparing_depth) {
            throw std::invalid_argument("Domain width is too small for the r_cutoff to cell size ratio."
                                        "Consider increasing the cell size or using ParticleContainer instead.");

        } else if(domain_max_dim[1] < comparing_depth) {
            throw std::invalid_argument("Domain height is too small for the r_cutoff to cell size ratio."
                                        "Consider increasing the cell size.");
        }

        three_dimensions = false;

    } else {
        if(domain_max_dim[0] <= comparing_depth) {
            throw std::invalid_argument("Domain width is too small for the r_cutoff to cell size ratio."
                                        "Consider increasing the cell size.");

        } else if(domain_max_dim[1] <= comparing_depth) {
            throw std::invalid_argument("Domain height is too small for the r_cutoff to cell size ratio."
                                        "Consider increasing the cell size.");

        }
        else if(domain_max_dim[2] < comparing_depth) {
            throw std::invalid_argument("Domain depth is too small for the r_cutoff to cell size ratio."
                                        "Consider increasing the cell size.");
        }

        three_dimensions = true;
    }
}

CellContainer::~CellContainer() {}

void CellContainer::setNextCell(std::array<dim_t, 3> &next_position) {
    static dim_t x = 1;
    static dim_t y = 1;
    static dim_t z = 1;

    //previous call set x to max, indicating the end
    if(x == dim_t_res) {
        next_position[0] = dim_t_res;
        x = 1;
        y = 1;
        z = 1;
        return;
    }

    while(particles[x][y][z].empty()) {

        if(x < domain_max_dim[0]) {
            ++x;

        }else if(y < domain_max_dim[1]) {
            x = 1;
            ++y;

        } else if(z < domain_max_dim[2]) {
            x = 1;
            y = 1;
            ++z;

        } else {
            //done
            next_position[0] = dim_t_res;
            x = 1;
            y = 1;
            z = 1;
            return;
        }
    }

    next_position[0] = x;
    next_position[1] = y;
    next_position[2] = z;

    if (x < domain_max_dim[0]) {
        ++x;
    }else if (y < domain_max_dim[1]) {
        x = 1;
        ++y;
    } else if (z < domain_max_dim[2]) {
        x = 1;
        y = 1;
        ++z;
    } else {
        //done
        x = dim_t_res;
    }
}

enum direction_status {
    first_subset, second_subset, third_subset
};






void CellContainer::setNext3dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start) {
    static direction_status status = first_subset;

    switch (status) {
        case (first_subset):
            // (0, 0, 1 to depth)
            if (pattern[2] < comparing_depth) {
                ++pattern[2];
            } else {
                pattern[0] = 1;
                pattern[2] = -comparing_depth;
                status = second_subset;
            }
            break;

        case (second_subset):
            // (1 to depth, 0, -depth to depth)
            if (pattern[0] < comparing_depth) {
                ++pattern[0];

            } else if (pattern[2] < comparing_depth) {
                pattern[0] = 1;
                ++pattern[2];

            } else {
                pattern[0] = -comparing_depth;
                pattern[1] = 1;
                pattern[2] = -comparing_depth;
                status = third_subset;
            }
            break;

        case (third_subset):
            // (-depth to depth, 1 to depth, -depth to depth)
            if (pattern[0] < comparing_depth) {
                ++pattern[0];

            } else if (pattern[1] < comparing_depth) {
                pattern[0] = -comparing_depth;
                ++pattern[1];

            } else if (pattern[2] < comparing_depth) {
                pattern[0] = -comparing_depth;
                pattern[1] = 1;
                ++pattern[2];
            } else {
                //finished
                pattern[0] = 0;
                pattern[1] = 0;
                pattern[2] = 0;
                status = first_subset;
                start[0] = dim_t_res;
                return;
            }
            break;
    }
}

void CellContainer::setNext2dPattern(std::array<dim_t, 3> &pattern, std::array<dim_t, 3> &start) {
    static direction_status status = first_subset;

    switch (status) {
        case (first_subset):
            // (1 to depth, 0, 0)
            if (pattern[0] < comparing_depth) {
                ++pattern[0];

            } else {
                pattern[0] = -comparing_depth;
                pattern[1] = 1;
                status = second_subset;
            }
            break;

        case (second_subset):
            // (-depth to depth, 1 to depth, 0)
            if (pattern[0] < comparing_depth) {
                ++pattern[0];

            } else if (pattern[1] < comparing_depth) {
                pattern[0] = -comparing_depth;
                ++pattern[1];

            } else {
                //finished
                pattern[0] = 0;
                pattern[1] = 0;
                pattern[2] = 0;
                status = first_subset;
                start[0] = dim_t_res;
                return;
            }
            break;
    }
}

void CellContainer::setNextPath(std::array<dim_t, 3> &start, std::array<dim_t, 3> &pattern) {
    static std::array<dim_t, 3> current_pattern{0,0,0};
    static std::array<dim_t, 3> begin{1,1,1};
    static std::array<dim_t, 3> end(domain_max_dim);
    static dim_t tmp_x = 0;
    static dim_t tmp_y = 0;
    static dim_t tmp_z = 0;
    static dim_t s_x, s_y, s_z;

    if(tmp_x == 0 && tmp_y == 0 && tmp_z == 0) {
        //get next pattern

        if(three_dimensions) {
            setNext3dPattern(current_pattern, start);
        } else {
            setNext2dPattern(current_pattern, start);
        }

        tmp_x = current_pattern[0];
        tmp_y = current_pattern[1];
        tmp_z = current_pattern[2];
        s_x = 1;
        s_y = 1;
        s_z = 1;
        begin = {1,1,1};
        end = domain_max_dim;
    }

    pattern[0] = current_pattern[0];
    pattern[1] = current_pattern[1];
    pattern[2] = current_pattern[2];

    //get next start
    if(tmp_x != 0) {
        start[1] = s_y;
        start[2] = s_z;

        if(0 < tmp_x) {
            start[0] = tmp_x;
            begin[0] = current_pattern[0] + 1;
        } else {
            start[0] = tmp_x + domain_max_dim[0] + 1;
            end[0] += current_pattern[0];
        }

        if(s_y < end[1]) {
            ++s_y;

        } else if(s_z < end[2]) {
            s_y = 1;
            ++s_z;

        } else {
            s_y = 1;
            s_z = 1;

            if(0 < tmp_x) {
                s_x = begin[0];
                --tmp_x;
            } else {
                ++tmp_x;
            }
        }
    } else if(tmp_y != 0) {
        start[0] = s_x;
        start[2] = s_z;

        if(0 < tmp_y) {
            start[1] = tmp_y;
            begin[1] = current_pattern[1] + 1;
        } else {
            start[1] = tmp_y + domain_max_dim[1] + 1;
            end[1] += current_pattern[1];
        }

        if(s_x < end[0]) {
            ++s_x;

        } else if(s_z < end[2]) {
            s_x = begin[0];
            ++s_z;

        } else {
            s_x = begin[0];
            s_z = 1;

            if(0 < tmp_y) {
                s_y = begin[1];
                --tmp_y;
            } else {
                ++tmp_y;
            }
        }
    } else if(tmp_z != 0) {
        start[0] = s_x;
        start[1] = s_y;

        if(0 < tmp_z) {
            start[2] = tmp_z;
        } else {
            start[2] = tmp_z + domain_max_dim[2] + 1;
        }

        if(s_x < end[0]) {
            ++s_x;

        } else if(s_y < end[1]) {
            s_x = begin[0];
            ++s_y;

        } else {
            s_x = begin[0];
            s_y = begin[1];

            if(0 < tmp_z) {
                --tmp_z;
            } else {
                ++tmp_z;
            }
        }
    }
}

/**Boundary Iterator methods: */

void CellContainer::next_correct_boundary_index(dim_t &x,
                                                                dim_t &y,
                                                                dim_t &z) {

    auto domain_max = domain_max_dim;                                                                    
  if (z == 1 || z == domain_max[2]) {
    if (x < domain_max[0]) {
      ++x;
    } else if (y < domain_max[1]) {
      x = 1;
      ++y;
    } else {
      x = 1;
      y = 1;
      ++z;
    }

  } else if (1 < z && z < domain_max[2]) {
    if (y == 1) {
      if (x < domain_max[0]) {
        ++x;
      } else {
        ++y;
        x = 1;
      }
    } else if (1 < y && y < domain_max[1]) {
      if (x == 1) {
        x = domain_max[0];
      } else
      // x == domain_max[0] in the else case
      {
        y++;
        x = 1;
      }
    } else if (y == domain_max[1]) {
      if (x < domain_max[0]) {
        x++;
      } else {
        x = 1;
        y = 1;
        z++;
      }
    }
  }
}


CellContainer::BoundaryIterator &CellContainer::BoundaryIterator::operator++() {
  std::cout << "Domain max: [" << cell.domain_max_dim[0] << " " <<  cell.domain_max_dim[1] << " " <<  cell.domain_max_dim[2] << "]";

  if (!(z < cell.domain_max_dim[2])) {
    std::cout << "z is already too big" << std::endl;
    x = -1;
    y = 1;
    z = 1;
    return *this;
  }
  while (cell.particles[x][y][z].empty()) {
    std::cout << "apparently this is empty : [" << x << ", " << y << " ," << z << "] \n";
    // changes the variables inplace to be the next correct boundary
    cell.next_correct_boundary_index(x, y, z);
    std::cout << "After finding non empty : [" << x << ", " << y << " ," << z << "] \n";
  }
  if (!(z < cell.domain_max_dim[2])) {
    x = -1;
    y = 1;
    z = 1;
    return *this;
  }
  cell.next_correct_boundary_index(x,y,z);

  return *this;
}


std::vector<Particle*>& CellContainer::BoundaryIterator::operator*(){
  return cell.particles[x][y][z];
}

bool CellContainer::BoundaryIterator::operator==(const BoundaryIterator& other){
    return (x == other.x && y == other.y && z == other.z);
}

bool CellContainer::BoundaryIterator::operator!=(const BoundaryIterator& other){
    return (!(x == other.x && y == other.y && z == other.z));
}

CellContainer::BoundaryIterator CellContainer::begin_boundary(){
    return CellContainer::BoundaryIterator(*this);
}

CellContainer::BoundaryIterator CellContainer::end_boundary(){
    return CellContainer::BoundaryIterator(*this,-1,1,1);
}



void CellContainer::addParticle(std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg) {
    if(domain_bounds[0] <= x_arg[0] || domain_bounds[1] <= x_arg[1] || domain_bounds[2] <= x_arg[2] ||
        x_arg[0] < 0 || x_arg[1] < 0 || x_arg[2] < 0) {
        std::cout << "wanted to add " << x_arg[0] << " , " << x_arg[1] << " , " << x_arg[2] << "\n";
        std::cout << "into " << domain_bounds[0] << " , " << domain_bounds[1] << " , " << domain_bounds[2] << " \n";
        throw std::invalid_argument("The provided coordinates are outside the domain borders.");
    }
    std::cout << "Adding Particle: " << x_arg[0] << " , " << x_arg[1] << " , " << x_arg[2] << "\n";
    particle_instances.emplace_back(x_arg, v_arg, m_arg);
    

}

void CellContainer::allocateCell(std::array<double, 3> &x, std::array<dim_t , 3> &cell_position) {
    cell_position[0] = static_cast<dim_t>(x[0] / cell_size + 1);
    cell_position[1] = static_cast<dim_t>(x[1] / cell_size + 1);
    cell_position[2] = static_cast<dim_t>(x[2] / cell_size + 1);
}

void CellContainer::createPointers(){
    for(Particle& particle : particle_instances){
        static std::array<dim_t , 3> pos;
        std::array<double,3> x_arg = particle.getX();
        allocateCell(x_arg, pos);
        particles.at(pos[0]).at(pos[1]).at(pos[2]).push_back(&particle);
        particle_amount++;
    }
}


void CellContainer::plotParticles(outputWriter::VTKWriter &writer) {
    std::array<dim_t, 3> current_position;
  
    setNextCell(current_position);

    while(current_position[0] != dim_t_res) {
        std::vector<Particle*> &current_cell = particles[current_position[0]][current_position[1]][current_position[2]];
        for(Particle* particle : current_cell){
            writer.plotParticle(*particle);
        }
        setNextCell(current_position);
    }

}




std::string CellContainer::to_string() {
  std::ostringstream out_str;  
  

  out_str << "There are in total " << ((domain_max_dim[0]+1) * (domain_max_dim[1]+1) * (domain_max_dim[2]+1)) << " Cells" << std::endl;
  out_str << "The actual domain has " << ((domain_max_dim[0]-1) * (domain_max_dim[1]-1) * (domain_max_dim[2]-1)) << " Cells" << std::endl;
  out_str << "The actual domain has  \n" << (domain_max_dim[0]+1) <<  " cells in x dir. \n" << (domain_max_dim[1]+1) << " cells in y dir. \n"  << (domain_max_dim[2]+1) << " cells in z dir." << std::endl;
  out_str << "The but the domain is from  \nx: 1  - " << (domain_max_dim[0]) <<  "\ny: 1 - " << (domain_max_dim[1]) << "\ny: 1 - "  << (domain_max_dim[2]) <<  std::endl;


  std::array<dim_t, 3> current_position;
  setNextCell(current_position);  
  while(current_position[0] != dim_t_res){
    out_str << "The cell with index x=" << current_position[0] << " y=" << current_position[1] << " z=" << current_position[2] << std::endl;
    out_str << "Has the following Particles: " << std::endl;

    for(auto& particle : particles[current_position[0]][current_position[1]][current_position[2]]){
      out_str << (*particle).toString() << std::endl;
    }
    out_str << "\n\n";
    setNextCell(current_position);  
  }

  
  return out_str.str();
}

size_t CellContainer::size() {
    return particle_amount;
}







