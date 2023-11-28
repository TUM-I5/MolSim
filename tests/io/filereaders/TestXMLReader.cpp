#include <gtest/gtest.h>

#include "data/FileLoader.h"
#include "io/input/xml/XMLFileReader.h"
#include "particles/containers/directsum/DirectSumContainer.h"
#include "utils/ArrayUtils.h"

#define EXPECT_CONTAINS_POS_NEAR(list, point, tol) \
    EXPECT_TRUE(std::find_if(list.begin(), list.end(), [&](auto& x) { return ArrayUtils::L2Norm(x - point) < tol; }) != list.end());

TEST(XMLFileReader, CorrectParticleContainer) {
    std::unique_ptr<ParticleContainer> particle_container = std::make_unique<DirectSumContainer>();
    XMLFileReader file_reader;
    SimulationParams conf = file_reader.readFile(FileLoader::get_test_file_path("test_collision.xml"), particle_container);

    double err = 1e-13;
    EXPECT_EQ(conf.video_length, 10);
    EXPECT_EQ(conf.fps, 24);
    EXPECT_NEAR(conf.end_time, 100, err);
    EXPECT_NEAR(conf.delta_t, 0.0014, err);

    EXPECT_EQ(particle_container->size(), 10 * 2 * 4 + 3 * 3 * 3 + 1);

    EXPECT_EQ(conf.output_format, FileOutputHandler::OutputFormat::VTK);
    EXPECT_EQ(conf.container_type.index(), 0);

    for (int i = 0; i < 80; i++) {
        EXPECT_NEAR((*particle_container)[i].getM(), 1, err);
    }
    for (int i = 80; i < 107; i++) {
        EXPECT_NEAR((*particle_container)[i].getM(), 1.5, err);
        EXPECT_EQ(2, (*particle_container)[i].getType());
    }
    EXPECT_EQ(10, (*particle_container)[107].getType());
    EXPECT_NEAR((*particle_container)[107].getM(), 100, err);

    EXPECT_NEAR((*particle_container)[0].getX()[0], 0, err);
    EXPECT_NEAR((*particle_container)[0].getX()[1], 0, err);
    EXPECT_NEAR((*particle_container)[0].getX()[2], 0, err);

    EXPECT_NEAR((*particle_container)[80].getX()[0], 100, err);
    EXPECT_NEAR((*particle_container)[80].getX()[1], 20, err);
    EXPECT_NEAR((*particle_container)[80].getX()[2], 20, err);

    EXPECT_NEAR((*particle_container)[107].getX()[0], 20, err);
    EXPECT_NEAR((*particle_container)[107].getX()[1], 100, err);
    EXPECT_NEAR((*particle_container)[107].getX()[2], 100, err);
}
