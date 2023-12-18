

#include <iostream>
#include "CellContainerIterators.h"
#include <stdexcept>

void CellContainer::CustomIterator::next_correct_index_custom(dim_t& x, dim_t& y, dim_t& z){
  if(x ==-1){
    throw new std::invalid_argument("Trying to increment an end pointer");
  }
  if(x < upper_x){
    x++;
  }else if(y < upper_y){
    x=lower_x;
    y++;
  }else if(z < upper_z){
    x=lower_x;
    y=lower_y;
    z++;
  }else{
    x = -1;
    y = 1;
    z = 1;
  }

}


CellContainer::CustomIterator &CellContainer::CustomIterator::operator++() {
  //std::cout << "Domain max: [" << cell.domain_max_dim[0] << " " <<  cell.domain_max_dim[1] << " " <<  cell.domain_max_dim[2] << "]";
  next_correct_index_custom(x,y,z);
  return *this;
}


std::vector<Particle*>& CellContainer::CustomIterator::operator*(){
  return cell.particles[x][y][z];
}

bool CellContainer::CustomIterator::operator==(const CellContainer::CustomIterator& other){
    return (x == other.x && y == other.y && z == other.z);
}

bool CellContainer::CustomIterator::operator!=(const CellContainer::CustomIterator& other){
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

