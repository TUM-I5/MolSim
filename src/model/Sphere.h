/*
 * Sphere.h
 *
 *  Created on: 01.12.2022
 *      Author: wohlrapp
 */

#pragma once

#include "spdlog/spdlog.h"

#include <array>

/**
 * @brief Sphere data class, holds all the information about a sphere
 */
class Sphere
{
private:
    // coordinate of the center
    std::array<double, 3> center;

    // radius in terms of the number of the particles
    int r;

    // distance between the particles (mesh width of the grid)
    double h;

    // mass of one particle
    double m;

    // initial velocity of the particles
    std::array<double, 3> v;

    // mean-value of the velocity of the brownian motion
    double meanV;

    // type of the particles
    int type;

    /**
     * a speedlog logger which logs construction and destruction of particles
     */
    std::shared_ptr<spdlog::logger> _memoryLogger;

public:
    /**
     * @brief Construct a new Sphere object
     *
     * @param center The coordinate of the center
     * @param r The radius in terms of the number of the particles
     * @param h The distance between the particles (mesh width of the grid)
     * @param m The mass of one particle
     * @param v The initial velocity of the particles
     * @param meanV The average velocity of the brownian motion
     * @param type The type of the particles
     */
    Sphere(std::array<double, 3> center, int r, double h, double m, std::array<double, 3> v, double meanV, int type);

    ~Sphere();

    /*
     * Getters
     */

    const std::array<double, 3> getCenter();

    const int getR();

    const double getH();

    const double getM();

    const std::array<double, 3> getV();

    const double getMeanV();

    const int getType();

    /*
     * Setters
     */

    const void setCenter(std::array<double, 3> &center);

    const void setR(int r);

    const void setH(double h);

    const void setM(double m);

    const void setV(std::array<double, 3> &v);

    const void setMeanV(double meanV);

    const void setType(int type);
};
