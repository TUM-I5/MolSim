#include <gtest/gtest.h>

#include <fstream>

#include "data/FileLoader.h"
#include "io/FileUtils.h"
#include "io/output/FileOutputHandler.h"
#include "particles/ParticleContainer.h"
#include "utils/ArrayUtils.h"

/*
 * Test if the VTKWriter writes the correct data into the file.
 */
TEST(VTKWriter, CorrectWritingOfParticles) {
    ParticleContainer particle_container;

    for (double i = 0; i < 5; i++) {
        auto pos = std::array<double, 3>{i, 2 * i, 3 * i};
        auto vel = std::array<double, 3>{4 * i, 5 * i, 6 * i};
        particle_container.addParticle(Particle(pos, vel, i, i));
    }

    auto output_folder = FileLoader::get_test_file_path("temp/VTKWriterTest");
    FileOutputHandler file_output_handler{FileOutputHandler::OutputFormat::VTK, output_folder};

    file_output_handler.writeFile(0, particle_container);

    // load the file
    std::ifstream file(output_folder + "/MD_VTK_0000.vtu");
    std::stringstream buffer;
    buffer << file.rdbuf();

    // check if the file contains the correct data

    // clang-format off
    std::string expected =  MULTILINE(
    <?xml version="1.0" encoding="UTF-8" standalone="no" ?>
    <VTKFile byte_order="LittleEndian" type="UnstructuredGrid" version="0.1">
    <UnstructuredGrid>
        <Piece NumberOfCells="0" NumberOfPoints="5">
        <PointData>
            <DataArray Name="mass" NumberOfComponents="1" format="ascii" type="Float32">0 1 2 3 4</DataArray>
            <DataArray Name="velocity" NumberOfComponents="3" format="ascii" type="Float32">0 0 0 4 5 6 8 10 12 12 15 18 16 20 24</DataArray>
            <DataArray Name="force" NumberOfComponents="3" format="ascii" type="Float32">0 0 0 0 0 0 0 0 0 0 0 0 0 0 0</DataArray>
            <DataArray Name="type" NumberOfComponents="1" format="ascii" type="Int32">0 1 2 3 4</DataArray>
        </PointData>
        <CellData/>
        <Points>
            <DataArray Name="points" NumberOfComponents="3" format="ascii" type="Float32">0 0 0 1 2 3 2 4 6 3 6 9 4 8 12</DataArray>
        </Points>
        <Cells>
            <DataArray Name="types" NumberOfComponents="0" format="ascii" type="Float32"/>
        </Cells>
        </Piece>
    </UnstructuredGrid>
    </VTKFile>
    );
    // clang-format on

    EXPECT_EQ(remove_whitespace(buffer.str()), remove_whitespace(expected));
}
