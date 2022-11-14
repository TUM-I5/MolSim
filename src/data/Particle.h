/*
 * Particle.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#pragma once

#include <array>
#include <string>
#include "Eigen"

class Particle {

private:
    /**
     * Position of the particle
     */
    Eigen::Vector3d x;

    /**
     * Velocity of the particle
     */
    Eigen::Vector3d v;

    /**
     * Force effective on this particle
     */
    Eigen::Vector3d f;

    /**
     * Force which was effective on this particle
     */
    Eigen::Vector3d old_f;

    /**
     * Mass of this particle
     */
    double m;

    /**
     * Type of the particle. Use it for whatever you want (e.g. to separate
     * molecules belonging to different bodies, matters, and so on)
     */
    int type;

public:
    explicit Particle(int type=0);

    Particle(const Particle &other);

    Particle(
            // for visualization, we need always 3 coordinates
            // -> in case of 2d, we use only the first and the second
            const std::array<double, 3>& x_arg, const std::array<double, 3>& v_arg, double m_arg,
            int type = 0);

    Particle(const Eigen::Vector3d& x_arg, const Eigen::Vector3d& v_arg, double m_arg, int type = 0);

    Particle(const Eigen::Vector3d& x_arg, const std::array<double, 3>& v_arg, double m_arg, int type = 0);


    virtual ~Particle();

    const Eigen::Vector3d &getX() const;

    const Eigen::Vector3d &getV() const;

    const Eigen::Vector3d &getF() const;

    const Eigen::Vector3d &getOldF() const;

    /**
     * @brief returns the mass of the particle
     *
     * @return double
     */
    double getM() const;


    /**
     * @brief Returns the Type of the Particle
     *
     * @return int
     */
    int getType() const;

    /**
     * @brief sets the force the particle experiences
     *
     */
    void setF(const Eigen::Vector3d &);

    /**
     * @brief Set the old force the particle experiences
     * This parameter is useful because we need it to compute the new velocity and the new coordinates
     *
     */
    void setOldF(const Eigen::Vector3d &);

    /**
     * @brief F = F + given_parameter
     *
     */
    void add_to_F(const Eigen::Vector3d &);

    /**
     * @brief X = X + given_parameter
     *
     */
    void add_to_X(const Eigen::Vector3d &);

    /**
     * @brief V = V + given_parameter
     *
     */
    void add_to_V(const Eigen::Vector3d &);

    bool operator==(const Particle &other);

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, Particle &p);
