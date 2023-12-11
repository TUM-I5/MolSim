#include <gtest/gtest.h>
#include "inputHandling/FileReader.h"
#include "inputHandling/generators/CuboidGeneration.h"
#include "inputHandling/generators/SphereGeneration.h"
#include "particleModel/storage/CellContainer.h"
#include "particleModel/storage/CellContainerIterators.h"
#include "particleModel/updating/CellCalculator.h"
#include "utils/ArrayUtils.h"


TEST(test_Thermo_Stat,test_basic){
    CellContainer container(15,15,15,3.0,3.0);
    CellCalculator calculator(container,0.0014,"LennJones",
        {boundary_conditions::outflow,boundary_conditions::outflow,
        boundary_conditions::outflow,boundary_conditions::outflow,
        boundary_conditions::outflow,boundary_conditions::outflow},0,30);

    container.addParticle({1,1,0},{2,2,2},3);
    container.addParticle({6,5,0},{2,2,2},4);
    container.addParticle({7,12,0},{2,2,2},4);
    container.addParticle({7,4,0},{3,4,5},7);

    container.createPointers();

    std::cout << container.to_string() << std::endl;

    double temp = 0;




    //after this call the Temperature of the system should be 30 (because it's the target temperature)
    calculator.applyThermostats();

    //particles afterwards
    std::cout << container.to_string() << std::endl;

    temp = 0;

    for(auto iter = container.begin(); iter != container.end(); ++iter){
        for(Particle* particle_ptr : *iter){   
            //std::cout << "encountered particle"  << std::endl;
            auto v = particle_ptr->getV();
            temp += ((v[0]* v[0] + v[1] * v[1] + v[2] * v[2] ) * particle_ptr->getM());
        }
    }

    std::cout << "Afterwards have kinetic energy(times 2): " << temp << std::endl;

    //dimension should be 2 and boltzman constant is 1
    temp = temp / ( 3.0 * container.size() *  1);


    ASSERT_NEAR(30,temp,0.00001);

}

TEST(test_Thermo_Stat,test_initial_Temp){
     FileReader::SphereData sphere = {
        {20, 20, 20}, 
        {0, 0, 0}, 
        1.0,            
        5,             
        1.0,             
        1,             
        1              
    };

    // Initialize CuboidData with specific values
    FileReader::CuboidData cuboid = {
        {10, 10, 10}, 
        {0, 0, 0}, 
        5,               
        5,               
        5,               
        1,            
        1,             
        0.3,             
        0.6,             
        0              
    };
    
    FileReader::ProgramArgs args = {
        false,              
        0.054,           // delta_t
        50.0,          // t_end
        2.0,            //cut of radius
        2.0,            //cell size
        30.0,       //initial temp
        std::nullopt,       //max temp diff
        std::nullopt,       //target temp
        50,          //thermostat write frequency
        {boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective
        },    //boundary conditions
        {100,100,100},        //domain size
        "out",          // file_basename
        10,             // write_frequency
        {cuboid},      // spheres
        {sphere}       // cuboids
    };

    CellContainer cellContainer(args.domain_dimensions[0],args.domain_dimensions[1],args.domain_dimensions[2],args.cut_of_radius,args.cell_size);

    FileReader::initializeCorrectInitialTemp(args);
    
    std::cout << args.to_string() << std::endl;

    addCuboids(cellContainer,args.cuboids);
    addSpheres(cellContainer,args.spheres,2);

    cellContainer.createPointers();

    std::cout << cellContainer.to_string() << std::endl;

    double temp = 0;

    for(auto iter = cellContainer.begin(); iter != cellContainer.end(); ++iter){
        for(Particle* particle_ptr : *iter){   
            //std::cout << "encountered particle"  << std::endl;
            auto v = particle_ptr->getV();
            temp += ((v[0]* v[0] + v[1] * v[1] + v[2] * v[2] ) * particle_ptr->getM());
        }
    }

    std::cout << "Afterwards have kinetic energy(times 2): " << temp << std::endl;

    //dimension should be 3 and boltzman constant is 1
    temp = temp / ( 3.0 * cellContainer.size() *  1);

    std::cout << "temp is: " << temp << std::endl;
    std::cout << "temp should be roughly 30" << std::endl;




}









