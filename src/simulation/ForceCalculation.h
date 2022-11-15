
#pragma once

#include "../model/ParticleContainer.h"

class ForceCalculation{
public: 
    virtual void calculateForce(ParticleContainer &particleContainer) = 0; 
};