#include <gtest/gtest.h>
#include "particleModel/storage/CellContainer.h"
#include "particleModel/storage/CellContainerIterators.h"
#include "particleModel/updating/CellCalculator.h"
#include "utils/ArrayUtils.h"


TEST(test_Thermo_Stat,test_basic){
    CellContainer container(15,15,15,3.0,3.0);
    CellCalculator calculator(container,0.0014,"LennJones",
        {boundary_conditions::outflow,boundary_conditions::outflow,
        boundary_conditions::outflow,boundary_conditions::outflow,
        boundary_conditions::outflow,boundary_conditions::outflow},30);

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









