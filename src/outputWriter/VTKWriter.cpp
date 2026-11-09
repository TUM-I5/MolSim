/*
 * VTKWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */
#ifdef ENABLE_VTK_OUTPUT

#include "VTKWriter.h"

#include <spdlog/spdlog.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>

#include <iomanip>
#include <sstream>

#include "ParticleContainer.h"

namespace output_writer {

void VTKWriter::Write(const ParticleContainer &particles, const int iteration) {
    // Initialize points
    const auto points = vtkSmartPointer<vtkPoints>::New();

    // Create and configure data arrays
    const vtkNew<vtkFloatArray> mass_array;
    mass_array->SetName("mass");
    mass_array->SetNumberOfComponents(1);

    const vtkNew<vtkFloatArray> velocity_array;
    velocity_array->SetName("velocity");
    velocity_array->SetNumberOfComponents(3);

    const vtkNew<vtkFloatArray> force_array;
    force_array->SetName("force");
    force_array->SetNumberOfComponents(3);

    const vtkNew<vtkIntArray> type_array;
    type_array->SetName("type");
    type_array->SetNumberOfComponents(1);

    for (auto p : particles) {
        points->InsertNextPoint(p.x().data());
        mass_array->InsertNextValue(static_cast<float>(p.m()));
        velocity_array->InsertNextTuple(p.v().data());
        force_array->InsertNextTuple(p.f().data());
        type_array->InsertNextValue(p.type());
    }

    // Set up the grid
    const auto grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    grid->SetPoints(points);

    // Add arrays to the grid
    grid->GetPointData()->AddArray(mass_array);
    grid->GetPointData()->AddArray(velocity_array);
    grid->GetPointData()->AddArray(force_array);
    grid->GetPointData()->AddArray(type_array);

    // Create filename with iteration number
    std::stringstream strstr;
    strstr << prefix_ << "_" << std::setfill('0') << std::setw(4) << iteration << ".vtu";

    // Create writer and set data
    const vtkNew<vtkXMLUnstructuredGridWriter> writer;
    auto path = dir_ / strstr.str();
    writer->SetFileName(path.string().c_str());
    writer->SetInputData(grid);
    if (ascii_)
        writer->SetDataModeToAscii();
    else
        writer->SetDataModeToBinary();

    // Write the file
    bool write_success = writer->Write() == 1;

    if (!write_success) {
        SPDLOG_ERROR("Failed writing VTK file to {}", path.string());
    } else {
        SPDLOG_DEBUG("Wrote {}", path.string());
    }
}
}  // namespace output_writer
#endif
