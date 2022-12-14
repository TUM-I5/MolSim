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
     * Variable for the Lennard Jones force
     */
    double epsilon;

    /**
     * Variable for the Lennard Jones force
     */
    double sigma;

    /**
     * Type of the particle. Use it for whatever you want (e.g. to separate
     * molecules belonging to different bodies, matters, and so on)
     */
    int type;

    /**
     * Cell to which particle belongs. If DirectSum is used, all particles belong to cell 0
     */
    int cell_idx;

    /**
     * True if particle is out of cell bounds
     */
    bool invalid;

    /**
     * True if particle is out of domain bounds
     */
    bool halo;

    /**
     * a speedlog logger which logs construction and destruction of particles
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:
    explicit Particle(int type = 0);

    Particle(const Particle &other);

    /**
     * @brief Creates a new particle and adds it to the vector
     * @param x_arg The position array of the particle
     * @param v_arg The velocity array of the particle
     * @param m_arg The mass of the particle
     * @param type_arg The type of the particle
     * @param epsilon_arg The epsilon of the particle
     * @param sigma_arg The sigma of the particle
     */
    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, double m_arg, double epsilon_arg, double sigma_arg, int type = 0);

    /**
     * @brief Creates a new particle and adds it to the vector
     * @param x_arg The position array of the particle
     * @param v_arg The velocity array of the particle
     * @param f_arg The force acting on the particle
     * @param old_f_arg The previous force acting on the particle
     * @param m_arg The mass of the particle
     * @param type_arg The type of the particle
     * @param epsilon_arg The epsilon of the particle
     * @param sigma_arg The sigma of the particle
     */
    Particle(
        // for visualization, we need always 3 coordinates
        // -> in case of 2d, we use only the first and the second
        std::array<double, 3> x_arg, std::array<double, 3> v_arg, std::array<double, 3> f_arg, std::array<double, 3> old_f_arg, double m_arg, double epsilon_arg, double sigma_arg, int type = 0);

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

    const int getCellIdx();
    const void setCellIdx(int cell_idx_arg);

    const bool getInvalid();
    const void setInvalid(bool invalid_arg);

    const bool getHalo();
    const void setHalo(bool halo_arg);

    const double getEpsilon() const;
    const double getSigma() const;

    bool operator==(Particle &other);

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
