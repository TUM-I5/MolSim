#pragma once

#include <iostream>

#include "particles/Particle.h"

/**
 * @brief Interface for force source classes
 *
 * Definition of an interface for force source classes.
 * This is used to ensure inheriting classes implement the method calculateForce(ParticleContainer& particles)
 * according to our definition.
 *
 * A ForceSource specifies the force a particle q exerts on another particle p.
 */
class ForceSource {
   public:
    /**
     * @brief Virtual destructor for correct cleanup of derived classes
     *
     * Virtual destructor to ensure correct deletion of inheriting classes.
     */
    virtual ~ForceSource() = default;

    /**
     * @brief Calculates the force a particle q exerts on another particle p
     *
     * @param p Particle whose force is to be updated
     * @param q Particle which exerts the force on p
     * @return Force exerted by q on p
     *
     * Calculates the force a particle q exerts on another particle p.
     */
    virtual std::array<double, 3UL> calculateForce(Particle& p, Particle& q) const = 0;

    /**
     * @brief Conversion from a force source object to a string containing its name
     */
    virtual explicit operator std::string() const = 0;
};

// overload the << operator for the ForceSource class to allow easier printing
inline std::ostream& operator<<(std::ostream& os, const ForceSource& forceSource) {
    os << static_cast<std::string>(forceSource);
    return os;
}