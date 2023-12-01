#include <gtest/gtest.h>
#include "particleModel/storage/CellContainer.h"


/**
 * @brief
*/
TEST(cellcontainer, test_setNextCell) {
    CellContainer cellContainer0{2.0,2.0,2.0,3.0,3.0};
    CellContainer cellContainer1{2.0,2.0,2.0,3.0,3.0};
    CellContainer cellContainer2{9.0,9.0,9.0,3.0,3.0};
    std::array<dim_t,3> position{0,0,0};
    std::array<double,3> x111{0,0,0};
    std::array<double,3> x123{0,5,8};
    std::array<double,3> x333{8,8,8};
    std::array<double,3> v{0,0,0};
    double m = 1;

    //test correct iteration with empty CellContainer of domain size 1
    cellContainer0.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);
    cellContainer0.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);

    cellContainer1.addParticle(x111, v, m);
    //test correct iteration with CellContainer of domain size 1 and non-empty cells
    cellContainer1.setNextCell(position);
    ASSERT_EQ(position[0], 1);
    ASSERT_EQ(position[1], 1);
    ASSERT_EQ(position[2], 1);

    cellContainer1.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);

    cellContainer1.setNextCell(position);
    ASSERT_EQ(position[0], 1);
    ASSERT_EQ(position[1], 1);
    ASSERT_EQ(position[2], 1);

    cellContainer1.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);

    cellContainer2.addParticle(x111, v, m);
    cellContainer2.addParticle(x123, v, m);
    cellContainer2.addParticle(x333, v, m);
    //test correct iteration with CellContainer of domain size 9
    cellContainer2.setNextCell(position);
    ASSERT_EQ(position[0], 1);
    ASSERT_EQ(position[1], 1);
    ASSERT_EQ(position[2], 1);

    cellContainer2.setNextCell(position);
    ASSERT_EQ(position[0], 1);
    ASSERT_EQ(position[1], 2);
    ASSERT_EQ(position[2], 3);

    cellContainer2.setNextCell(position);
    ASSERT_EQ(position[0], 3);
    ASSERT_EQ(position[1], 3);
    ASSERT_EQ(position[2], 3);

    cellContainer2.setNextCell(position);
    ASSERT_EQ(position[0], dim_t_res);
}

/**
 * @brief
*/
TEST(cellcontainer, test_setNextPath) {
    CellContainer cellContainer0{2.0,2.0,2.0,3.0,3.0};
    CellContainer cellContainer1{2.0,2.0,2.0,3.0,3.0};
    CellContainer cellContainer2{9.0,9.0,9.0,3.0,3.0};
    std::array<dim_t,3> position{0,0,0};
    std::array<double,3> x111{0,0,0};
    std::array<double,3> x123{0,6,8};
    std::array<double,3> x333{8,8,8};
    std::array<double,3> v{0,0,0};
    double m = 1;

}