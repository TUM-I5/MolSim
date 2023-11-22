#pragma once

#include "particleModel/ParticleContainer.h"
#include "FileReader.h"
void addSpheres(ParticleContainer& particleContainer, std::list<FileReader::SphereData> spheres);

void generateSpheresMethod1(FileReader::SphereData& sphere, ParticleContainer& particleContainer);

void generateSpheresMethod2(FileReader::SphereData& sphere, ParticleContainer& particleContainer);