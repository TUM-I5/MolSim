#pragma once

#include "particleModel/storage/CellContainer.h"
#include "particleModel/updating/Calculator.h"



/**
 * using a lambda function interface for the calcualtion of the force between two
 * Particles, takes in two Particles and returns the vector3 of forces
 * acting between the two given Particles
 * simplified:
 * forceCalculation refers to such functions "std::array<double,3> func(const Particle&,const Particle&)"
 * uses constant references because forceCalculation mustn't change the Particles
 */
using ForceCalculation = std::function<std::array<double, 3>(const Particle &, const Particle &)>;

class CellCalculator : public Calculator{

public:
    CellCalculator(CellContainer &cellContainer, const double delta_t, ForceCalculation force);

    void initalize() override;

    void iteration() override;

    void initializeFX();

    void calculateLinkedCellF();

    void calculateWithinFVX();

    void calculateBoundaries();


private:

    void calculateBoundariesHorizontal(dim_t z,bool add, ForceCalculation_Ghost force);
    void calculateBoundariesVertical(dim_t x,bool add, ForceCalculation_Ghost force); //Front and Back
    void calculateBoundariesVertical2(dim_t y,bool add, ForceCalculation_Ghost force); //Left and Right

    CellContainer &cellContainer;
    std::vector<std::vector<std::vector<std::vector<Particle>>>> &particles;
};