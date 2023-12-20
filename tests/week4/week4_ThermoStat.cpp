#include <gtest/gtest.h>
#include "inputHandling/FileReader.h"
#include "inputHandling/generators/CuboidGeneration.h"
#include "inputHandling/generators/SphereGeneration.h"
#include "particleModel/storage/CellContainer.h"
#include "particleModel/storage/CellContainerIterators.h"
#include "particleModel/updating/CellCalculator.h"
#include "utils/ArrayUtils.h"


double getTemp(CellContainer& container){

    double temp = 0;

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

    return temp;
}


TEST(test_Thermo_Stat,test_basic){
    CellContainer container(15,15,15,3.0,3.0);
    CellCalculator calculator(container,0.0014,3.0,
        {boundary_conditions::outflow,boundary_conditions::outflow,
        boundary_conditions::outflow,boundary_conditions::outflow,
        boundary_conditions::outflow,boundary_conditions::outflow},0.0,30.0);

    container.addParticle({1,1,0},{2,2,2},3);
    container.addParticle({6,5,0},{2,2,2},4);
    container.addParticle({7,12,0},{2,2,2},4);
    container.addParticle({7,4,0},{3,4,5},7);

    container.createPointers();

    std::cout << container.to_string() << std::endl;

    
    //after this call the Temperature of the system should be 30 (because it's the target temperature)
    calculator.applyThermostats();

    //particles afterwards
    std::cout << container.to_string() << std::endl;

    double temp = getTemp(container);

    //due to rounding errors etc. we can't expect to get the exact double temperature again
    ASSERT_NEAR(30,temp,0.00001);

}


TEST(test_Thermo_Stat,test_heating){
    CellContainer container(50,50,50,3.0,3.0);
    CellCalculator calculator(container,0.0014,3.0,
        {boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective
        },30,100.0,10.0);  

    //max_temp_diff is 1 and target_temp is 30
    //so in every Thermostat iteration, the temperature is increased by one maximum


    //have some particles to simulate
    container.addParticle({1,1,0},{2,2,2},3);
    container.addParticle({6,5,0},{2,2,2},4);
    container.addParticle({7,12,0},{2,2,2},4);
    container.addParticle({7,4,0},{3,4,5},7);
    container.addParticle({20,30,0},{2,4,2},3);
    container.addParticle({3,5,45},{3,4,3},8);
    container.addParticle({24,8,4},{1,1,1},9);

    container.createPointers();

    double temp = getTemp(container);

    std::cout << "The Temperature before the simulation is: " << temp << std::endl; 
    //This will be 40

    calculator.initializeFX();


    for(int i = 0; i < 50; i++){
        calculator.applyReflectiveBoundaries();
        calculator.calculateLinkedCellF();
        calculator.calculateWithinFVX();
        calculator.applyThermostats();
        temp = getTemp(container);
        std::cout << "The current Temperature is: " << temp << std::endl; 
    }

    temp = getTemp(container);

    std::cout << "The Temperature after the simulation is: " << temp << std::endl;
    //due to rounding errors etc. we can't expect to get the exact double temperature again
    ASSERT_NEAR(100,temp,0.00001);
}


TEST(test_Thermo_Stat,test_cooling){
    CellContainer container(50,50,50,3.0,3.0);
    CellCalculator calculator(container,0.0014,3.0,
        {boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective
        },30,20.0,5.0);  

    //max_temp_diff is 1 and target_temp is 30
    //so in every Thermostat iteration, the temperature is increased by one maximum


    //have some particles to simulate
    container.addParticle({1,1,0},{2,2,2},3);
    container.addParticle({6,5,0},{2,2,2},4);
    container.addParticle({7,12,0},{2,2,2},4);
    container.addParticle({7,4,0},{3,4,5},7);
    container.addParticle({20,30,0},{2,4,2},3);
    container.addParticle({3,5,45},{3,4,3},8);
    container.addParticle({24,8,4},{1,1,1},9);

    container.createPointers();

    double temp = getTemp(container);

    std::cout << "The Temperature before the simulation is: " << temp << std::endl; 
    //This will be 40

    calculator.initializeFX();


    for(int i = 0; i < 20; i++){
        calculator.applyReflectiveBoundaries();
        calculator.calculateLinkedCellF();
        calculator.calculateWithinFVX();
        calculator.applyThermostats();
        temp = getTemp(container);
        std::cout << "The current Temperature is: " << temp << std::endl; 
    }

    temp = getTemp(container);

    std::cout << "The Temperature after the simulation is: " << temp << std::endl;
    //due to rounding errors etc. we can't expect to get the exact double temperature again
    ASSERT_NEAR(20,temp,0.00001);
}

TEST(test_Thermo_Stat,test_initial_Temp){
     FileReader::SphereData sphere = {
        {100, 100, 100}, 
        {0, 0, 0}, 
        1.0,            
        20,             
        1.0,             
        1,             
        1              
    };

    // Initialize CuboidData with specific values
    FileReader::CuboidData cuboid = {
        {20, 20, 20}, 
        {0, 0, 0}, 
        20,               
        20,               
        20,               
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
        0.0,            //gravity factor
        30.0,       //initial temp
        std::nullopt,       //max temp diff
        std::nullopt,       //target temp
        50,          //thermostat write frequency
        {boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective,
        boundary_conditions::reflective,boundary_conditions::reflective
        },    //boundary conditions
        {200,200,200},        //domain size
        std::nullopt,
        std::nullopt,
        "out",          // file_basename
        10,             // write_frequency
        {cuboid},      // spheres
        {sphere}       // cuboids
    };

    CellContainer cellContainer(args.domain_dimensions[0],args.domain_dimensions[1],args.domain_dimensions[2],args.cut_off_radius,args.cell_size);

    FileReader::initializeCorrectInitialTemp(args);
    
    std::cout << args.to_string() << std::endl;

    addCuboids(cellContainer,args.cuboids);
    addSpheres(cellContainer,args.spheres,2);

    cellContainer.createPointers();

    //std::cout << cellContainer.to_string() << std::endl;
    double temp = getTemp(cellContainer);


    // no comparison possible, because nondeterministic initialization
    // with boltzmann distribution
    std::cout << "temp is: " << temp << std::endl;
    std::cout << "temp should be roughly 30" << std::endl;




}









