#pragma once

#include "particleModel/storage/SimulationContainer.h"
#include "FileReader.h"
void addSpheres(SimulationContainer& particleContainer, std::list<FileReader::SphereData> spheres);

void generateSpheresMethod1(FileReader::SphereData& sphere, SimulationContainer& particleContainer);

void generateSpheresMethod2(FileReader::SphereData& sphere, SimulationContainer& particleContainer);