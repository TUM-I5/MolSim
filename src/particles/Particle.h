/**
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>

/**
 * @brief Class to represent a particle
 *
 * Class to represent a particle.
 * A particle has a position, a velocity, a mass and a type.
 * Additionally, the current and previous force exerted on the particle is stored.
 */
class Particle {
   private:
    /**
     * @brief Position of the particle
     */
    std::array<double, 3> x;

    /**
     * @brief Velocity of the particle
     */
    std::array<double, 3> v;

    /**
     * @brief Force effective on this particle
     */
    std::array<double, 3> f;

    /**
     * @brief Force which was effective on this particle
     */
    std::array<double, 3> old_f;

    /**
     * @brief Mass of this particle
     */
    double m;

    /**
     * @brief Type of the particle. Use it for whatever you want (e.g. to separate molecules belonging to different bodies, matters, and so
     * on)
     */
    int type;

   public:
    explicit Particle(int type = 0);

    Particle(const Particle& other);

    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg, int type = 0);

    virtual ~Particle();

    /**
     * @brief Sets the position of the particle
     *
     * @param x New position
     */
    void setX(const std::array<double, 3>& x);

    /**
     * @brief Sets the velocity of the particle
     *
     * @param v New velocity
     */
    void setV(const std::array<double, 3>& v);

    /**
     * @brief Sets the force of the particle
     *
     * @param f New force
     */
    void setF(const std::array<double, 3>& f);

    /**
     * @brief Sets the old force of the particle
     *
     * @param oldF New old force
     */
    void setOldF(const std::array<double, 3>& oldF);

    /**
     * @brief Gets the position of the particle
     */
    const std::array<double, 3>& getX() const;

    /**
     * @brief Gets the velocity of the particle
     */
    const std::array<double, 3>& getV() const;

    /**
     * @brief Gets the total force of the particle
     */
    const std::array<double, 3>& getF() const;

    /**
     * @brief Gets the old total force of the particle
     */
    const std::array<double, 3>& getOldF() const;

    /**
     * @brief Gets the mass of the particle
     */
    double getM() const;

    /**
     * @brief Gets the type of the particle
     */
    int getType() const;

    bool operator==(Particle& other);

    bool operator==(const Particle& other) const;

    std::string toString() const;
};

std::ostream& operator<<(std::ostream& stream, Particle& p);
