

#include <iostream>
#include "CellContainerIterators.h"
#include <stdexcept>
#include <cmath>



void CellContainer::BoundaryIterator::next_correct_boundary_index(dim_t &x,dim_t &y,dim_t &z, dim_t depth) {

  auto domain_max = cell.domain_max_dim;                          
  if(x ==-1){
    throw new std::invalid_argument("Trying to increment an end pointer");
  }
  if (z <= depth ||  (domain_max[2]-depth  < z &&  z <= domain_max[2])) {
    if (x < domain_max[0]) {
      ++x;
    } else if (y < domain_max[1]) {
      x = 1;
      ++y;
    } else {
      x = 1;
      y = 1;
      ++z; std::cout << "z was incremented at 1\n" ;
    }

  } else if (depth < z && z <= domain_max[2]-depth) {
    if (y <= depth || (domain_max[1] > y &&  y > domain_max[1] - depth)) {
      if (x < domain_max[0]) {
        ++x;
      } else {
        ++y;
        x = 1;
      }
    } else if (depth < y && y <= domain_max[1]-depth) {
      if (x < depth || (domain_max[0] > x  && x > domain_max[0]-depth)) {
        x++;
      } else if(domain_max[0]-depth >= x && x >= depth)
      {
        x = domain_max[0]-depth+1;
      }else {
        assert(x == domain_max[0]);
        //x = domain_max[0] from last time
        x = 1;
        y++;
      }
    } else  {
      assert(y == domain_max[1]);
      if (x < domain_max[0]) {
        ++x;
      } else {
        x = 1;
        y = 1;
        z++;
      }
    }
  }else{
    x = -1;
    y = 1;
    z = 1;
  }
  if(z > domain_max[2]){
    x = -1;
    y = 1;
    z = 1;
  }
}


CellContainer::BoundaryIterator &CellContainer::BoundaryIterator::operator++() {
  next_correct_boundary_index(x,y,z,depth);
  return *this;
}


std::vector<Particle*>& CellContainer::BoundaryIterator::operator*(){
  return cell.particles[x][y][z];
}

bool CellContainer::BoundaryIterator::operator==(const CellContainer::BoundaryIterator& other){
    return (x == other.x && y == other.y && z == other.z);
}

bool CellContainer::BoundaryIterator::operator!=(const CellContainer::BoundaryIterator& other){
    return (!(x == other.x && y == other.y && z == other.z));
}



void CellContainer::Iterator::next_correct_index(dim_t& x, dim_t& y, dim_t& z){
  if(x ==-1){
    throw new std::invalid_argument("Trying to increment an end pointer");
  }
  if(x < cell.domain_max_dim[0]){
    x++;
  }else if(y < cell.domain_max_dim[1]){
    x=1;
    y++;
  }else if(z < cell.domain_max_dim[2]){
    x=1;
    y=1;
    z++;
  }else{
    x = -1;
    y = 1;
    z = 1;
  }

}


CellContainer::Iterator &CellContainer::Iterator::operator++() {
  //std::cout << "Domain max: [" << cell.domain_max_dim[0] << " " <<  cell.domain_max_dim[1] << " " <<  cell.domain_max_dim[2] << "]";
  next_correct_index(x,y,z);
  return *this;
}


std::vector<Particle*>& CellContainer::Iterator::operator*(){
  return cell.particles[x][y][z];
}

bool CellContainer::Iterator::operator==(const CellContainer::Iterator& other){
    return (x == other.x && y == other.y && z == other.z);
}

bool CellContainer::Iterator::operator!=(const CellContainer::Iterator& other){
    return (!(x == other.x && y == other.y && z == other.z));
}

