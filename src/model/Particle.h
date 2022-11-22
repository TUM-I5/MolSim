/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include "spdlog/spdlog.h"

#include <array>
#include <string>
#include <memory>

class Particle
{

private:
    /**
     * Position of the particle
     */
    std::array<double, 3> x;

    /**
     * Velocity of the particle
     */
    std::array<double, 3> v;

    /**
     * Force effective on this particle
     */
    std::array<double, 3> f;

    /**
     * Force which was effective on this particle
     */
    std::array<double, 3> old_f;

    /**
     * Mass of this particle
     */
    double m;

    /**
     * Type of the particle. Use it for whatever you want (e.g. to separate
     * molecules belonging to different bodies, matters, and so on)
     */
    int type;

    /**
     * a speedlog logger which logs construction and destruction of particles
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:
    explicit Particle(int type = 0);

    Particle(const Particle &other);

    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg,
        int type = 0);

    virtual ~Particle();

    const std::array<double, 3> &getX() const;
    const void setX(const std::array<double, 3> &new_x);

    const std::array<double, 3> &getV() const;
    const void setV(const std::array<double, 3> &new_v);

    const std::array<double, 3> &getF() const;
    const void setF(const std::array<double, 3> &new_f);
    /**
     * adding force to the existing force vector
     * @param new_f The force that will be added to the existing force array
     */
    const void addF(const std::array<double, 3> &new_f);

    const std::array<double, 3> &getOldF() const;
    const void setOldF(const std::array<double, 3> &new_old_f);

    double getM() const;

    int getType() const;

    bool operator==(Particle &other);

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
