#include <gtest/gtest.h>
#include "particleModel/updating/CellCalculator.h"
#include "particleModel/storage/CellContainerIterators.h"

void calculateBoundariesTopOrBottom(dim_t lower_z,dim_t upper_z, double z_border,
                             CellCalculator& cellCalculator ){

    auto domain_max_dim = cellCalculator.getDomain_Max();
    auto particles = cellCalculator.getParticles();
    auto ref_size = cellCalculator.getRefSize();
    dim_t x, y;
    x = y =  1;

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
      double distance =z_dim - z_border;

      if (std::abs(distance) < ref_size) {
        // calculate repulsing force with Halo particle
        double ghost_particle_z = z_dim - 2*distance;

        std::array<double,3> F_particle_ghost = cellCalculator.ghostParticleLennardJonesForce(particle,{x_dim,y_dim,ghost_particle_z});
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


void calculateBoundariesFrontOrBack(dim_t lower_x,dim_t upper_x ,double x_border, dim_t z_until,
                                                    CellCalculator& cellCalculator){

    auto domain_max_dim = cellCalculator.getDomain_Max();
    auto particles = cellCalculator.getParticles();
    auto ref_size = cellCalculator.getRefSize();

    dim_t y, z;
    z = y =  1;

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

        std::array<double,3> F_particle_ghost = cellCalculator.ghostParticleLennardJonesForce(particle,{ghost_particle_x,y_dim,z_dim});
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

void calculateBoundariesLeftOrRight(dim_t lower_y,dim_t upper_y ,double y_border, dim_t z_until,
                                                    CellCalculator& cellCalculator){
    
    auto domain_max_dim = cellCalculator.getDomain_Max();
    auto particles = cellCalculator.getParticles();
    auto ref_size = cellCalculator.getRefSize();
    
    dim_t x, z;
    z = x =  1;

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

        std::array<double,3> F_particle_ghost = cellCalculator.ghostParticleLennardJonesForce(particle,{x_dim,ghost_particle_y,z_dim});
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






TEST(test_new_Boundaries,test_basic){
   CellContainer cellContainer(4,4,1,1.0,1.0);

    CellCalculator cellCalculator(cellContainer,0.0014,
            {boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective});

    //adding Partciles in every relevant region
    cellContainer.addParticle({0.1,0.1,0},{0,0,0},1); //corner
    cellContainer.addParticle({2,0.1,0},{0,0,0},1); //x-border in negative direction in the middle 
    cellContainer.addParticle({2,3.99,0},{0,0,0},1); //x-border in positive direction in the middle 
    cellContainer.addParticle({3.99,2,0},{0,0,0},1); //y-border in positive direction in the middle 
    cellContainer.addParticle({0.1,2,0},{0,0,0},1); //y-border in negative direction in the middle

    cellContainer.addParticle({2,2,0},{0,0,0},1); //particle not at the boundary (further away then 1.1225 from boundary)
    //all of their forces will be initalized to 0

    cellContainer.createPointers();

    /*
    setup a second equivalent cellContainer
    
    */


    CellContainer other_cellContainer(4,4,1,1.0,1.0);


    CellCalculator other_cellCalculator(other_cellContainer,0.0014,
            {boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective});


     //adding Partciles in every relevant region
    other_cellContainer.addParticle({0.1,0.1,0},{0,0,0},1); //corner
    other_cellContainer.addParticle({2,0.1,0},{0,0,0},1); //x-border in negative direction in the middle 
    other_cellContainer.addParticle({2,3.99,0},{0,0,0},1); //x-border in positive direction in the middle 
    other_cellContainer.addParticle({3.99,2,0},{0,0,0},1); //y-border in positive direction in the middle 
    other_cellContainer.addParticle({0.1,2,0},{0,0,0},1); //y-border in negative direction in the middle

    other_cellContainer.addParticle({2,2,0},{0,0,0},1); //particle not at the boundary (further away then 1.1225 from boundary)
    //all of their forces will be initalized to 0

    other_cellContainer.createPointers();

    std::cout << cellContainer.to_string() << std::endl;

    std::cout << other_cellContainer.to_string() << std::endl;

    cellCalculator.applyReflectiveBoundaries(); //now Boundary Conditions should have been applied to all particles

    // now do the same manually with the old functions
    // we know that reflective boundaries are applied on every side in 2D
    auto domain_max_dim = other_cellCalculator.getDomain_Max();
    auto domain_bounds = other_cellCalculator.getDomainBounds();
    auto comparing_depth = other_cellContainer.getComparingdepth();
    dim_t z_max = 1;

    calculateBoundariesFrontOrBack(domain_max_dim[0]-comparing_depth,domain_max_dim[0],
                                   domain_bounds[0],z_max,other_cellCalculator);

    calculateBoundariesFrontOrBack(1,comparing_depth+1,0,z_max,other_cellCalculator);

    calculateBoundariesLeftOrRight(domain_max_dim[1]-comparing_depth,domain_max_dim[1],
                                    domain_bounds[1],z_max,other_cellCalculator); 

    calculateBoundariesLeftOrRight(1,comparing_depth+1,0,z_max,other_cellCalculator); 

    auto iter = cellContainer.begin();
    auto other_iter = cellContainer.begin();

    for(; iter != cellContainer.end() && other_iter != other_cellContainer.end(); ++iter, ++other_iter){
        auto particle_iter = (*iter).begin();
        auto other_particle_iter = (*other_iter).begin();
        for(;particle_iter != (*iter).end() && other_particle_iter != (*other_iter).end();
                                                 ++particle_iter , ++other_particle_iter){
            Particle& particle = **particle_iter;
            Particle& other_particle = **other_particle_iter;
            std::cout << "Comparing particles: " << std::endl;
            std::cout << particle.toString() << std::endl;
            std::cout << other_particle.toString() << std::endl;
            //check that all particles are equal
            ASSERT_EQ(particle,other_particle);
        }
    }





}





