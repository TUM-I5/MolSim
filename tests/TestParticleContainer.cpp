#include <gtest/gtest.h>

#include "types/ParticleContainer.h"

TEST(ParticleContainer, Constructor) {
    ParticleContainer container;
    EXPECT_EQ(container.size(), 0);
}