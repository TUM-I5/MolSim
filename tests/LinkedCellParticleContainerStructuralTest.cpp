//
// Created by Feryal Ezgi on 4.12.2023.
//
#include "gtest/gtest.h"
#include "../src/models/LinkedCellParticleContainer.h"

class LinkedCellParticleContainerStructuralTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test constructor with default boundary behavior
TEST_F(LinkedCellParticleContainerStructuralTest, ConstructorTest) {
    LinkedCellParticleContainer container(10, 10, 10, 1, 0.1);

// Including halo cells it should be xSize + 2
    EXPECT_EQ(container.getXCells(), 12);
    EXPECT_EQ(container.getYCells(), 12);
    EXPECT_EQ(container.getZCells(), 12);
    EXPECT_EQ(container.getCutoffRadius(), 1);
    EXPECT_EQ(container.getDeltaT(), 0.1);

// if there is a deltaT value then all boundaries are set per default to be reflective
    EXPECT_EQ(container.getBoundaryBehaviorBack(), BoundaryBehavior::Reflective);
    EXPECT_EQ(container.getBoundaryBehaviorFront(), BoundaryBehavior::Reflective);


    LinkedCellParticleContainer containerDiffCellSize(6, 10, 10, 2, 0.1);
// Including halo cells and normalizing with the cellSize
// With xSize = 6, amount of xCells would be 6/2 + 2
    EXPECT_EQ(containerDiffCellSize.getXCells(), 5);
    EXPECT_EQ(containerDiffCellSize.getYCells(), 7);
    EXPECT_EQ(containerDiffCellSize.getZCells(), 7);
    EXPECT_EQ(containerDiffCellSize.getCutoffRadius(), 2);
}

// Test constructor with a specified boundary behavior for each individual border
TEST_F(LinkedCellParticleContainerStructuralTest, ConstructorWithBoundariesTest) {
    LinkedCellParticleContainer containerWithBoundaries(10, 10, 10, 1, 0.1,
                                                        BoundaryBehavior::Outflow,
                                                        BoundaryBehavior::Reflective,
                                                        BoundaryBehavior::Reflective,
                                                        BoundaryBehavior::Reflective,
                                                        BoundaryBehavior::Reflective,
                                                        BoundaryBehavior::Reflective);

    EXPECT_EQ(containerWithBoundaries.getBoundaryBehaviorTop(), BoundaryBehavior::Outflow);
    EXPECT_EQ(containerWithBoundaries.getBoundaryBehaviorBottom(), BoundaryBehavior::Reflective);
    EXPECT_EQ(containerWithBoundaries.getBoundaryBehaviorRight(), BoundaryBehavior::Reflective);
    EXPECT_EQ(containerWithBoundaries.getBoundaryBehaviorLeft(), BoundaryBehavior::Reflective);
    EXPECT_EQ(containerWithBoundaries.getBoundaryBehaviorFront(), BoundaryBehavior::Reflective);
    EXPECT_EQ(containerWithBoundaries.getBoundaryBehaviorBack(), BoundaryBehavior::Reflective);

}


TEST_F(LinkedCellParticleContainerStructuralTest, IndexConversionTest) {
    LinkedCellParticleContainer container(10, 10, 10, 1, 0.1);

    int cellIndex = container.index3dTo1d(1, 2, 3);
    EXPECT_EQ(cellIndex, 1 + 2 * container.getXCells() + 3 * container.getXCells() * container.getYCells());

    std::array<int, 3> coordinates = container.index1dTo3d(cellIndex);
    EXPECT_EQ(coordinates[0], 1);
    EXPECT_EQ(coordinates[1], 2);
    EXPECT_EQ(coordinates[2], 3);
}



