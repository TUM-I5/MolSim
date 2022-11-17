/*
 *  GravitationalForce.h
 *
 *  Created on: 17.11.2022
 *      Author: wohlrapp
 */

#pragma once 

#include "ForceCalculation.h"

class GravitationalForce : public ForceCalculation {
public: 
    void calculateForce(ParticleContainer &particleContainer) override; 
}; 