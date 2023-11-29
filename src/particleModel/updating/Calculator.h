
#pragma once

#include <functional>
#include <string>

#include "particleModel/storage/SimulationContainer.h"
#include "utils/ForceCalculations.h"


class Calculator {
    public:
  Calculator(const double delta_t, ForceCalculation force)
      :  delta_t(delta_t) , forceLambda(force) {}

  
  virtual void initalize() = 0;


  virtual void iteration() = 0;

 protected:
  const double delta_t;
  ForceCalculation forceLambda;
};