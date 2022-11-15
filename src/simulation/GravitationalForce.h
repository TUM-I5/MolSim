
#pragma once 

#include "ForceCalculation.h"

class GravitationalForce : public ForceCalculation {
public: 
    void calculateForce(ParticleContainer &particleContainer) override; 
}; 