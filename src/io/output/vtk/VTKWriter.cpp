

#include "VTKWriter.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "io/logger/Logger.h"

VTKFile_t VTKWriter::initializeOutput(int numParticles) const {
    VTKFile_t vtkFile("UnstructuredGrid");

    // per point, we add type, position, velocity and force
    PointData pointData;
    DataArray_t mass(type::Float32, "mass", 1);
    DataArray_t velocity(type::Float32, "velocity", 3);
    DataArray_t forces(type::Float32, "force", 3);
    DataArray_t type(type::Int32, "type", 1);
    pointData.DataArray().push_back(mass);
    pointData.DataArray().push_back(velocity);
    pointData.DataArray().push_back(forces);
    pointData.DataArray().push_back(type);

    CellData cellData;  // we don't have cell data => leave it empty

    // 3 coordinates
    Points points;
    DataArray_t pointCoordinates(type::Float32, "points", 3);
    points.DataArray().push_back(pointCoordinates);

    Cells cells;  // we don't have cells, => leave it empty
    // for some reasons, we have to add a dummy entry for paraview
    DataArray_t cells_data(type::Float32, "types", 0);
    cells.DataArray().push_back(cells_data);

    PieceUnstructuredGrid_t piece(pointData, cellData, points, cells, numParticles, 0);
    UnstructuredGrid_t unstructuredGrid(piece);
    vtkFile.UnstructuredGrid(unstructuredGrid);

    return vtkFile;
}

void VTKWriter::plotParticle(VTKFile_t& vtkFile, const Particle& p) const {
    if (!vtkFile.UnstructuredGrid().present()) {
        Logger::logger->error("VTKWriter: No UnstructuredGrid present");
        exit(-1);
    }

    PointData::DataArray_sequence& pointDataSequence = vtkFile.UnstructuredGrid()->Piece().PointData().DataArray();
    PointData::DataArray_iterator dataIterator = pointDataSequence.begin();

    dataIterator->push_back(p.getM());

    dataIterator++;
    dataIterator->push_back(p.getV()[0]);
    dataIterator->push_back(p.getV()[1]);
    dataIterator->push_back(p.getV()[2]);

    dataIterator++;
    dataIterator->push_back(p.getOldF()[0]);
    dataIterator->push_back(p.getOldF()[1]);
    dataIterator->push_back(p.getOldF()[2]);

    dataIterator++;
    dataIterator->push_back(p.getType());

    Points::DataArray_sequence& pointsSequence = vtkFile.UnstructuredGrid()->Piece().Points().DataArray();
    Points::DataArray_iterator pointsIterator = pointsSequence.begin();
    pointsIterator->push_back(p.getX()[0]);
    pointsIterator->push_back(p.getX()[1]);
    pointsIterator->push_back(p.getX()[2]);
}

void VTKWriter::writeFile(const std::string& output_dir_path, int iteration, const std::unique_ptr<ParticleContainer>& particle_container) const {
    auto filename = output_dir_path + "/" + "MD_VTK";

    std::stringstream strstr;
    strstr << filename << "_" << std::setfill('0') << std::setw(4) << iteration << ".vtu";

    auto vtkFile = initializeOutput(particle_container->size());

    for (const Particle& particle : *particle_container) {
        plotParticle(vtkFile, particle);
    }

    std::ofstream file(strstr.str().c_str());
    VTKFile(file, vtkFile);
    file.close();
}
