#include <gtest/gtest.h>
#include "particleModel/storage/CellContainer.h"
#include "particleModel/storage/CellContainerIterators.h"

#include "particleModel/updating/CellCalculator.h"


/**
 * @brief
 */
TEST(test_CellCalculation,test_periodic){
    CellContainer periodic{10, 10, 10, 2, 2};
    CellContainer not_periodic{10, 100, 10, 2, 2};

    std::array<double,3> x1{5,5,5};
    std::array<double,3> x2{5,6.1225,5};
    std::array<double,3> x3{5,7.245,5};
    std::array<double,3> v{0,5,0};
    double m = 1;
    std::array<

    periodic.addParticle(x1,v,m);
    periodic.addParticle(x2,v,m);
    periodic.addParticle(x3,v,m);
    not_periodic.addParticle(x1,v,m);
    not_periodic.addParticle(x2,v,m);
    not_periodic.addParticle(x3,v,m);
}