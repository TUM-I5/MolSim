#include <gtest/gtest.h>
#include "particleModel/storage/ParticleContainer.h"
#include "particleModel/updating/Model.h"
#include "utils/ForceCalculations.h"



/**
 * @brief   First a PartcleContainer is created with three Particles
 *          and then the Model is intialized with this Container.
 *          After that we call the calculateF(), calculateX() and calculateV()
 *          functions to see, if the Model acts on the particleContainer 
 *          as expected
*/
TEST(model,test_model_calculations){
    auto force_func = forceLennJonesPotentialFunction(1.0,5.0);
    double delta_t = 0.001;
    ParticleContainer particleContainer;
    Particle p0{{0,0,0},{0,1,0},1};
    Particle p1{{0,1,0},{0,-1,0},1};
    Particle p2{{0,0,1},{0,0,-1},1};
    particleContainer.addParticle({0,0,0},{0,1,0},1); //p
    particleContainer.addParticle({0,1,0},{0,-1,0},1);//p1
    particleContainer.addParticle({0,0,1},{0,0,-1},1);//p2
    //just to be sure that they are at the right place(bc im paranoid)
    particleContainer[0] = p0;  
    particleContainer[1] = p1;
    particleContainer[2] = p2;    
    Model M(particleContainer,delta_t,forceLennJonesPotentialFunction(1.0,5.0));

    

    M.calculateF();
    auto F_01 = force_func(p0,p1);
    auto F_12 = force_func(p1,p2);
    auto F_02 = force_func(p0,p2);
    auto F_0 = F_01 + F_02;
    auto F_1 = F_12 -F_01;
    auto F_2 = (-1*F_12) - F_02; 

    std::cout << "particleContainer[0].getF(): " << particleContainer[0].getF() << std::endl;
    std::cout << "Expected F_0: " << F_0 << std::endl;

    std::cout << "particleContainer[1].getF(): " << particleContainer[1].getF() << std::endl;
    std::cout << "Expected F_1: " << F_1 << std::endl;

    std::cout << "particleContainer[2].getF(): " << particleContainer[2].getF() << std::endl;
    std::cout << "Expected F_2: " << F_2 << std::endl;

    ASSERT_EQ(particleContainer[0].getF(),F_0);
    ASSERT_EQ(particleContainer[1].getF(),F_1);
    ASSERT_EQ(particleContainer[2].getF(),F_2);

    M.shiftForces();
    M.calculateX();
    auto calcX = [delta_t](Particle p,std::array<double,3> F ){
        std::array<double,3> res;
        for(int i = 0; i < 3; i++){
            res[i] = p.getX()[i] + (delta_t * p.getV()[i]) +  ( delta_t * delta_t * F[i]) * 1.0/(2.0 * p.getM());
        }
        return res;
    };
    auto X_0 = calcX(p0,F_0);
    auto X_1 = calcX(p1,F_1);
    auto X_2 = calcX(p2,F_2);

    p0.setX(0,X_0[0]);
    p0.setX(1,X_0[1]);
    p0.setX(2,X_0[2]);
    p1.setX(0,X_1[0]);
    p1.setX(1,X_1[1]);
    p1.setX(2,X_1[2]);
    p2.setX(0,X_2[0]);
    p2.setX(1,X_2[1]);
    p2.setX(2,X_2[2]);


    std::cout << "particleContainer[0].getX(): " << particleContainer[0].getX() << std::endl;
    std::cout << "Expected X_0: " << X_0 << std::endl;

    std::cout << "particleContainer[1].getX(): " << particleContainer[1].getX() << std::endl;
    std::cout << "Expected X_1: " << X_1 << std::endl;

    std::cout << "particleContainer[2].getX(): " << particleContainer[2].getX() << std::endl;
    std::cout << "Expected X_2: " << X_2 << std::endl;

    ASSERT_EQ(particleContainer[0].getX(),X_0);
    ASSERT_EQ(particleContainer[1].getX(),X_1);
    ASSERT_EQ(particleContainer[2].getX(),X_2);

    M.calculateF();

    auto new_F_01 = force_func(p0,p1);
    auto new_F_12 = force_func(p1,p2);
    auto new_F_02 = force_func(p0,p2);
    auto new_F_0 = new_F_01 + new_F_02;
    auto new_F_1 = new_F_12 - new_F_01;
    auto new_F_2 = (-1* new_F_12) - new_F_02; 

    auto V_0 = p0.getV() + (delta_t/2*p0.getM()) * (F_0 + new_F_0);
    auto V_1 = p1.getV() + (delta_t/2*p1.getM()) * (F_1 + new_F_1);
    auto V_2 = p2.getV() + (delta_t/2*p2.getM()) * (F_2 + new_F_2);
 

    M.calculateV();

    ASSERT_EQ(particleContainer[0].getV(),V_0);
    ASSERT_EQ(particleContainer[1].getV(),V_1);
    ASSERT_EQ(particleContainer[2].getV(),V_2);

}




